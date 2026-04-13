# Combat and Magic Systems

## Combat Engine (`src/fight.c`)

### Violence Update Loop

`violence_update()` is called every `PULSE_VIOLENCE` (3 seconds) from the game loop. For each character in `char_list` who is fighting:

1. Check that the character is still in the same room as their target
2. If the character is awake: call `multi_hit()` to resolve attacks
3. Trigger MudProgs: `rprog_rfight_trigger`, `mprog_hitprcnt_trigger`, `mprog_fightgroup_trigger`, `mprog_fight_trigger`
4. Call `check_assist()` to have allies join the fight

### Attack Resolution

**`multi_hit(ch, victim, dt)`** calculates how many attacks a character gets per round:

1. Primary weapon attack via `one_hit()`
2. If dual wielding: second weapon attack via `one_hit()`
3. Haste: one additional attack
4. `second_attack` skill: chance for another attack
5. `third_attack` skill: chance for another attack
6. `fourth_attack` skill: chance for another attack

For NPCs, `mob_hit()` adds:
- Area attacks (damage all enemies in room)
- Offensive flag behaviors: `OFF_BASH`, `OFF_BERSERK`, `OFF_DISARM`, `OFF_KICK`, `OFF_KICK_DIRT`, `OFF_TRIP`

**`one_hit(ch, victim, dt)`** resolves a single attack:

1. Calculate THAC0 (To Hit Armor Class 0) from class table, interpolated by level
2. Apply weapon skill bonus
3. Apply hitroll bonus
4. Roll d20 against target's AC for the appropriate damage type
5. On hit:
   - Roll weapon damage dice
   - Apply strength bonus (damroll)
   - Apply enhanced damage skill
   - Apply weapon special effects:
     - **Vorpal**: chance of instant kill
     - **Vampiric**: heal attacker for 25% of damage
     - **Flaming**: bonus fire damage
     - **Frost**: bonus cold damage
     - **Shocking**: bonus lightning damage
     - **Poison**: apply poison affect
   - Call `damage()` to apply the result
6. On miss: check parry, dodge, and shield block

### Defense Checks

| Check | Skill | Effect |
|---|---|---|
| `check_parry()` | `gsn_parry` | Deflect attack with weapon |
| `check_dodge()` | `gsn_dodge` | Evade attack entirely |
| `check_block()` | `gsn_shield_block` | Block with shield |

Each check uses the defender's skill percentage with randomization. Successful defense triggers `check_improve()` for skill improvement.

### Damage Application

`damage(ch, victim, weapon, dam, dt, class)`:

1. Safety checks: both characters exist, are in the same room, victim is not in a safe room
2. Damage reductions:
   - **Sanctuary**: halves all damage
   - **Protective shield**: further reduction
   - **Immunity/Resistance/Vulnerability**: `check_immune()` adjusts damage by type
3. Apply damage to victim's HP
4. `update_pos()` recalculates position:
   - HP > 0: maintain current position
   - HP 0 to -2: mortal wound
   - HP -3 to -5: incapacitated
   - HP -6 to -9: stunned
   - HP <= -10 (NPCs) or <= -11 (PCs): dead
5. Display damage messages via `dam_message()`
6. If victim dies: call death handler

### Damage Types

Damage types determine which AC category is used and which immunity/resistance/vulnerability applies:

| Category | Damage Types |
|---|---|
| Pierce | pierce, bite, stab, sting |
| Bash | bash, pound, crush, suction, beating |
| Slash | slash, slice, claw, cleave, scratch |
| Exotic | magic, fire, cold, lightning, acid, poison, negative, holy, energy, mental, disease, drowning, light, other, charm |

### Death Handling

Several death handlers exist:

- **`raw_kill(victim)`**: Standard NPC death -- creates corpse, transfers inventory, gives XP to group, extracts mob
- **`pk_kill(ch, victim)`**: PK death -- creates PC corpse, tracks kills/deaths, extracts but preserves character data
- **`chaos_kill(ch, victim)`**: Chaos mode death -- gives chaos score, teleports victim to recall (no real death)

**`make_corpse(ch)`**: Creates an NPC corpse object, moves all of the mob's inventory into the corpse, and places it in the room. Sets a timer for corpse decay.

**`death_cry(ch)`**: Sends a random gory message to the room and adjacent rooms. May create body part objects (severed head, torn heart, sliced arm, etc.).

### Experience Points

**`group_gain(ch, victim)`**: Distributes XP to the group that killed a mob:

1. Count group members in room and total levels
2. For each member, calculate individual XP via `xp_compute()`
3. Apply XP via `gain_exp()` with auto-level announcements

**`xp_compute(gch, victim, total_levels, members)`**: XP formula considers:

- Base XP from victim level
- Alignment matching (extra for opposite alignment kills)
- Level difference bonus/penalty
- Group size scaling
- Minimum 5% of base, maximum 200% of level-based limit

### Combat Initiation

- `set_fighting(ch, victim)`: Links ch->fighting to victim. If victim isn't fighting, sets victim->fighting to ch.
- `stop_fighting(ch, fBoth)`: Clears fighting state. If `fBoth`, also stops the opponent.

### Safety Checks

- `is_safe(ch, victim)`: Returns TRUE if PvP combat should be prevented (level 5+ restriction, safe rooms, chaos mode exception)
- `is_safe_spell(ch, victim, area)`: Same but for spell targeting
- `check_killer(ch, victim)`: Flags a player as a PKer if they attack another player

### Hatred System

When `AUTO_HATE` is defined:
- `start_hating(ch, victim)`: NPC memorizes a hate target
- `is_hating(ch, victim)`: Check if mob hates someone
- `stop_hating(ch)`: Clear hate memory

In `mobile_update()`, mobs check their hate memory and hunt hated targets.

## Combat Skills

| Skill | Function | Description |
|---|---|---|
| Backstab | `do_backstab` | Must be hidden/unseen. Multiplied damage (2x-5x by level). Single attack. |
| Circle | `do_circle` | Must be in combat. Enhanced backstab. Requires someone else tanking. |
| Bash | `do_bash` | Shield bash. Stuns target (WAIT_STATE). Chance based on skill. |
| Kick | `do_kick` | Kick during combat. Moderate damage. |
| Dirt kick | `do_dirt` | Kick dirt in opponent's eyes. Applies blindness. |
| Trip | `do_trip` | Trip opponent. Target falls (WAIT_STATE). |
| Berserk | `do_berserk` | Self-buff: +hitroll, +damroll, -AC, resist spell. Costs mana. |
| Disarm | `do_disarm` | Knock weapon from opponent's hand. |
| Blackjack | `do_blackjack` | Knock out opponent. Applies sleep affect. |
| Flee | `do_flee` | Attempt to run from combat. Random direction. XP penalty. |
| Rescue | `do_rescue` | Take hits for an ally. Swap fighting targets. |
| Weapon cleave | `do_weapon` | Destroy opponent's weapon with axe/sword. |
| Shield cleave | `do_shield` | Destroy opponent's shield with axe/sword. |

## Magic System (`src/magic.c`)

### Spell Casting

`do_cast(ch, argument)`:

1. Parse spell name and optional target
2. Look up spell in `skill_table[]` via `skill_lookup()`
3. Validate: level requirement, mana, position, not in combat (for some)
4. Calculate mana cost: `mana_cost = base_mana * 100 / (2 + (ch_level - spell_level))`
5. Resolve target based on spell's `target` type
6. Deduct mana
7. If casting in combat and recently damaged: chance of spell failure
8. Apply WAIT_STATE (lag)
9. Call `skill_table[sn].spell_fun(sn, level, ch, vo)`
10. If offensive spell: initiate combat if not already fighting
11. Call `check_improve()` for skill improvement

### Spell Target Types

| Type | Description |
|---|---|
| `TAR_IGNORE` | No target needed (area effects, self-only) |
| `TAR_CHAR_OFFENSIVE` | Target one enemy |
| `TAR_CHAR_DEFENSIVE` | Target one ally (or self) |
| `TAR_CHAR_SELF` | Self only |
| `TAR_OBJ_INV` | Target an object in inventory |
| `TAR_OBJ_CHAR_DEF` | Target object or defensive character |
| `TAR_OBJ_CHAR_OFF` | Target object or offensive character |

### Magic Items

`obj_cast_spell(sn, level, ch, victim, obj)` handles casting from objects:

- **Potions** (`do_quaff`): Drink to trigger up to 3 spell effects on self
- **Scrolls** (`do_recite`): Recite to trigger up to 3 spell effects. Can fail (scroll crumbles).
- **Wands** (`do_zap`): Point at a target. Uses charges. Zap to trigger one spell.
- **Staves** (`do_brandish`): Brandish for area effect. Uses charges.
- **Pills** (`do_eat`): Eat to trigger up to 3 spell effects on self

### Spell Categories

#### Offensive Spells

Deal damage to targets. Examples: `magic_missile`, `fireball`, `lightning_bolt`, `chain_lightning`, `meteor_swarm`, `acid_blast`, `colour_spray`. Dragon breath attacks: `fire_breath`, `frost_breath`, `acid_breath`, `gas_breath`, `lightning_breath`.

#### Defensive/Buff Spells

Apply beneficial affects. Examples: `armor` (+AC), `bless` (+hitroll/saves), `shield` (+AC), `sanctuary` (half damage), `stone_skin` (+AC), `haste` (+attacks/-AC), `fly`, `pass_door`, `giant_strength` (+STR), `frenzy` (+hitroll/damroll/saves).

#### Healing Spells

Restore HP. Progression: `cure_light` < `cure_serious` < `cure_critical` < `heal` < `restoration`. Also: `cure_blindness`, `cure_poison`, `cure_disease`, `remove_curse`, `refresh` (restore movement), `mass_healing`.

#### Debuff Spells

Apply harmful affects. Examples: `blindness`, `curse` (-hitroll/-saves), `poison` (periodic damage), `plague` (spreads), `sleep`, `charm_person`, `weaken` (-STR), `web` (prevents movement).

#### Utility Spells

Detection: `detect_evil`, `detect_hidden`, `detect_invis`, `detect_magic`, `detect_poison`, `identify`, `know_alignment`.

Transportation: `teleport`, `gate`, `nexus` (portal pair), `summon`, `word_of_recall`.

Creation: `create_food`, `create_water`, `create_spring`, `continual_light`.

Other: `enchant_weapon`, `enchant_armor`, `dispel_magic`, `cancellation`, `calm`, `faerie_fire`, `faerie_fog`.

### Saving Throws

`saves_spell(level, victim)`: Base 50% chance, modified by:
- Level difference: +5% per level of caster above victim
- Victim's `saving_throw` stat
- Capped at 5% minimum and 95% maximum

### Spell Affects

Most buff/debuff spells work by adding an `AFFECT_DATA` to the target:

```c
AFFECT_DATA af;
af.type      = sn;              // Spell number
af.level     = level;           // Caster level
af.duration  = level / 6;       // Ticks until expiry
af.location  = APPLY_AC;        // What to modify
af.modifier  = -20;             // How much
af.bitvector = 0;               // AFF_ flag to set
affect_to_char(victim, &af);
```

Affects are automatically removed when their duration expires in `char_update()`.

## Skill System (`src/skills.c`)

### Skill Improvement

`check_improve(ch, sn, success, multiplier)`:

Called after every skill use. Calculates improvement chance based on:
- Intelligence: `int_app[get_curr_stat(ch, STAT_INT)].learn`
- Skill rating for class: `skill_table[sn].rating[ch->Class]`
- Current proficiency: harder to improve at higher levels
- Multiplier: different skills improve at different rates

On improvement: +1 to `ch->pcdata->learned[sn]`, small XP award.

### Experience Per Level

`exp_per_level(ch, points)` returns the XP needed for the next level:

- Levels 1-100 follow an exponential curve (750 at level 1 up to 999999999 at level 92+)
- Modified by creation points spent: more points = higher XP requirement
- Modified by racial class multiplier from `pc_race_table`

### Skill Groups

Skills are organized into groups for character creation:
- `base_group`: Free skills every character of the class gets
- `default_group`: Standard package (costs creation points)
- Additional groups can be purchased during creation or learned from trainers

`do_gain` allows learning new skills/groups post-creation from trainer mobs using training sessions.

## Random Object Generation (`src/random.c`)

NPCs can spawn with procedurally generated equipment:

`load_random_objs(mob, mobIndex)`:
1. For each random object slot (up to `rnd_obj_num`):
2. Select a category from `rnd_obj_types` flags
3. Generate an item appropriate for the mob's level:
   - **Weapons**: Material + weapon type, damage dice scaled by level
   - **Armor**: Material + armor type, AC scaled by level
   - **Rings**: Material + descriptor, one random magical affect
   - **Bags**: Material + type, capacity based on material
4. Magical variants get bonus affects (stat bonuses, hitroll, damroll)
5. Equip the generated item on the mob

Materials scale with level: rusty -> iron -> steel -> silver -> mithril -> diamond -> "kick-ass" and beyond.
