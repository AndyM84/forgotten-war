# Memory Management and Utilities

EmberMUD uses a multi-layered memory management system designed to minimize fragmentation and avoid calling `free()` on game data structures. The implementation spans `ssm.c` (shared strings), `mem.c` (typed free-list allocators), `recycle.c` (validated recyclable structures), and portions of `db.c` (low-level allocators).

## Memory Architecture Overview

```
+--------------------------------------------------+
|  Application Layer (game code)                    |
|  str_dup() / free_string()    -- shared strings   |
|  new_xxx() / free_xxx()       -- typed objects     |
|  new_buf() / free_buf()       -- dynamic buffers   |
+--------------------------------------------------+
|  Allocator Layer                                  |
|  alloc_perm()   -- permanent pool (never freed)   |
|  alloc_mem()    -- temporary allocations          |
+--------------------------------------------------+
|  Shared String Heap (SSM)                         |
|  Reference-counted, defragmentable                |
|  Falls back to malloc() on overflow               |
+--------------------------------------------------+
|  C Runtime                                        |
|  calloc() for SSM heap init                       |
|  malloc() for SSM overflow only                   |
+--------------------------------------------------+
```

## Shared String Manager (`src/ssm.c`)

SSM v2.2 provides a reference-counted string pooling system. Since MUD worlds contain thousands of duplicate strings (room descriptions reused across instances, keyword lists, etc.), SSM dramatically reduces memory usage by storing only one copy of each unique string.

### Heap Structure

The SSM heap is a contiguous block of memory divided into fixed-size chunks, managed as a singly-linked free list.

```c
#define CHUNK_SIZE  0xfff0   // 65520 bytes per chunk

struct BufEntry {
    BufEntry *next;          // Link to next block
    unsigned short size;     // Usable data size
    short usage;             // Reference count (-1 = temp free, 0 = free, >0 = in use)
    char buf[1];             // String data begins here
};
```

Key properties:
- **Reference counting**: Each string tracks how many pointers reference it (max 32766)
- **Alignment**: All allocations rounded to machine word boundary (4 or 8 bytes)
- **First-fit allocation**: `str_dup()` walks the free list looking for the first block large enough
- **Block splitting**: If a free block is larger than needed, the excess is split into a new free block

### Core Functions

#### `void init_string_space()`

Called once at boot. Allocates the entire SSM heap via `calloc()`, links all chunks into a free list, and initializes the temporary hash table for boot-time deduplication.

#### `char *str_dup(const char *str)`

The primary string allocation function. Returns a pointer to the string in shared space.

1. Returns `&str_empty[0]` for NULL or empty strings (no allocation)
2. If the string is already in shared space, increments the reference count and returns the existing pointer
3. Otherwise, walks the free list for a first-fit block
4. If no block found and `numFree >= MAX_FREE` (1000), triggers `defrag_heap()` and retries
5. Falls back to `malloc()` when shared space is exhausted (tracked as overflow)

#### `void free_string(char **strptr)`

Decrements reference count. Frees the block when count reaches zero.

- Takes a pointer-to-pointer and sets the original to NULL
- Validates the pointer is within shared space bounds
- Detects double-frees (usage < 0) and prevents corruption
- For overflow strings (allocated via `malloc()`), calls `free()`

#### `char *fread_string(FILE *fp)`

Reads a tilde-delimited (`~`) string from an area file and allocates it in shared space. During boot, uses a temporary hash table to deduplicate strings as they are loaded, so identical strings across different areas share the same memory.

#### `int defrag_heap()`

Merges adjacent free blocks to reduce fragmentation. Walks the entire heap, combining neighboring free blocks whose total size fits within `CHUNK_SIZE`. Returns the number of merges performed.

### Boot-Time Optimization

During `boot_db()`, SSM maintains a temporary hash table (`temp_string_hash`) that maps string content to existing allocations. When `fread_string()` encounters a string that already exists in the hash, it returns the existing pointer (incrementing its reference count) instead of allocating a new block. After boot completes, `boot_done()` frees the hash table.

### Overflow Handling

When the shared space heap is full and defragmentation cannot free enough space, `str_dup()` falls back to `malloc()`. These overflow strings are tracked by two counters:

- `nOverFlowString` -- count of overflow allocations
- `sOverFlowString` -- total bytes of overflow allocations

`free_string()` detects overflow strings by checking if the pointer falls outside the shared space bounds, and calls `free()` instead of returning to the heap.

## Free-List Allocators (`src/mem.c`)

Every major game data structure has a dedicated `new_xxx()` / `free_xxx()` pair that uses a typed free list. Structures are never returned to the C runtime; instead they are pushed onto a per-type free list for reuse.

### Pattern

All allocators follow the same pattern:

```c
// Allocation
SOME_DATA *new_some(void) {
    SOME_DATA *p;
    if (some_free == NULL) {
        p = alloc_perm(sizeof(*p));  // First-time allocation
        top_some++;
    } else {
        p = some_free;               // Reuse from free list
        some_free = some_free->next;
    }
    // Initialize all fields...
    return p;
}

// Deallocation
void free_some(SOME_DATA *p) {
    free_string(&p->name);          // Release shared strings
    // Cascade-free sub-structures...
    p->next = some_free;            // Push to free list head
    some_free = p;
}
```

### Available Allocators

| Allocator | Structure | Free List | Notes |
|---|---|---|---|
| `new_reset_data` / `free_reset_data` | `RESET_DATA` | `reset_free` | Room reset commands |
| `new_mudprog` / `free_mudprog` | `MPROG_DATA` | `mprog_free` | MudProg scripts |
| `new_mudprog_group` / `free_mudprog_group` | `MPROG_GROUP` | `mprog_group_free` | MudProg groups |
| `new_area` / `free_area` | `AREA_DATA` | `area_free` | Area/zone definitions |
| `new_exit` / `free_exit` | `EXIT_DATA` | `exit_free` | Room exits |
| `new_extra_descr` / `free_extra_descr` | `EXTRA_DESCR_DATA` | `extra_descr_free` | Extra descriptions |
| `new_room_index` / `free_room_index` | `ROOM_INDEX_DATA` | `room_index_free` | Room templates |
| `new_affect` / `free_affect` | `AFFECT_DATA` | `affect_free` | Classic affects |
| `new_newaffect` / `free_newaffect` | `NEWAFFECT_DATA` | `newaffect_free` | Extended affects |
| `new_shop` / `free_shop` | `SHOP_DATA` | `shop_free` | Shop definitions |
| `new_obj_index` / `free_obj_index` | `OBJ_INDEX_DATA` | `obj_index_free` | Object templates |
| `new_mob_index` / `free_mob_index` | `MOB_INDEX_DATA` | `mob_index_free` | Mob templates |

### Cascading Cleanup

Complex structures perform cascading cleanup when freed. For example, `free_room_index()`:

1. Frees all string fields via `free_string()`
2. Walks and frees all exits via `free_exit()`
3. Walks and frees all extra descriptions via `free_extra_descr()`
4. Walks and frees all resets via `free_reset_data()`
5. Walks and frees all MudProg lists via `free_mem()`
6. Walks and frees all MudProg group lists via `free_mem()`
7. Pushes the room structure onto `room_index_free`

Similarly, `free_mob_index()` cascades to free shop data, MudProg lists, and MudProg group lists.

### Utility: `remove_color()`

`mem.c` also contains `remove_color()`, which strips backtick color codes from strings. It returns a pointer to a static buffer with color codes (`` `r ``, `` `b ``, `` `G ``, etc.) removed.

## Recyclable Structures (`src/recycle.c`)

This module extends the free-list pattern with validation checking via `VALIDATE` / `INVALIDATE` macros, and provides a dynamic buffer pool.

### Validation Pattern

```c
VALIDATE(ptr);      // Set a magic marker on the structure
IS_VALID(ptr);      // Check if the magic marker is intact
INVALIDATE(ptr);    // Clear the magic marker
```

This protects against double-frees and use-after-free bugs. When `free_xxx()` is called on an already-freed structure, `IS_VALID()` returns false and the function returns immediately.

### Ban Data (`BAN_DATA`)

```c
BAN_DATA *new_ban(void);
void free_ban(BAN_DATA *ban);
```

Allocates/recycles ban structures with validation. Uses a static `ban_zero` template for efficient initialization (`*ban = ban_zero`).

### Dynamic Buffer Pool (`BUFFER`)

The buffer system provides variable-sized string buffers that grow automatically. Buffers are used throughout the codebase for building output strings that may vary in length.

#### Size Categories

Buffers are allocated in power-of-two-like size buckets:

| Index | Size (bytes) |
|---|---|
| 0 | 16 |
| 1 | 32 |
| 2 | 64 |
| 3 | 128 |
| 4 | 256 |
| 5 | 1024 |
| 6 | 2048 |
| 7 | 4096 |
| 8 | 8192 |
| 9 | 16384 |

#### Buffer States

| State | Meaning |
|---|---|
| `BUFFER_SAFE` | Normal operation |
| `BUFFER_OVERFLOW` | Append exceeded maximum size; buffer is locked |
| `BUFFER_FREED` | Buffer has been returned to the free list |

#### Functions

| Function | Description |
|---|---|
| `new_buf()` | Allocate a buffer with default size (16 bytes) |
| `new_buf_size(int size)` | Allocate a buffer with a specific initial size |
| `free_buf(BUFFER *buffer)` | Return buffer to the free list |
| `add_buf(BUFFER *buffer, char *string)` | Append a string, resizing if needed |
| `clear_buf(BUFFER *buffer)` | Reset buffer to empty |
| `buf_string(BUFFER *buffer)` | Get pointer to the buffer's string content |

#### Growth Behavior

When `add_buf()` needs more space:

1. Calculate the required size: `strlen(current) + strlen(new) + 1`
2. Find the next size bucket via `get_size()` that fits
3. Allocate new backing storage via `alloc_mem()`
4. Copy existing content, free old storage
5. Concatenate the new string

If the required size exceeds the maximum bucket (16384), the buffer enters `BUFFER_OVERFLOW` state and all further appends return FALSE.

## Low-Level Allocators (`src/db.c`)

### `void *alloc_perm(int sMem)`

Permanent memory allocator. Allocates from a large pre-allocated block and never frees. Used by all `new_xxx()` functions for first-time allocations.

- Maintains a static pointer into the permanent memory block
- Rounds allocations up for alignment
- Tracks total via `sAllocPerm` and count via `nAllocPerm`

### `void *alloc_mem(int sMem)`

General-purpose allocator for temporary memory. Used by the BUFFER system for backing storage and other transient allocations.

### `void free_mem(void *pMemPtr)`

Frees memory allocated by `alloc_mem()`. Unlike the typed free-list functions, this actually releases memory (or returns it to a general pool depending on configuration).

## Memory Reporting

The `do_memory` immortal command (`memory` in-game) displays:

- Number and size of permanent allocations (`nAllocPerm` / `sAllocPerm`)
- Number and size of shared strings (`nAllocString` / `sAllocString`)
- Overflow string count and size
- Per-type counters (`top_affect`, `top_area`, `top_exit`, `top_mob_index`, `top_obj_index`, `top_reset`, `top_room`, `top_mprog`, etc.)
- Free list depths for major types

## Design Rationale

The memory system reflects several pragmatic design choices:

1. **No general-purpose free**: The free-list pattern avoids heap fragmentation that would occur from frequent malloc/free cycles during area resets, mob respawns, and object creation/destruction.

2. **Reference-counted strings**: MUD worlds are text-heavy with massive duplication. SSM ensures that 10,000 mobs with the description "A city guard stands here" share one copy.

3. **Boot-time deduplication**: The temporary hash table during loading prevents duplicate string allocations before the world is fully loaded, when duplication is highest.

4. **Typed free lists**: Each structure type has its own free list, so allocation is O(1) on reuse with no search overhead. The LIFO (stack) discipline means recently-freed structures (likely still in CPU cache) are reused first.

5. **Validation markers**: The VALIDATE/INVALIDATE pattern in `recycle.c` catches common bugs (double-free, use-after-free) at minimal runtime cost.
