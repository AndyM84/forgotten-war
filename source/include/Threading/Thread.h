#pragma once

#include <Common/Types.h>

typedef void* fwlptrvoid;

#if defined(FW_WINDOWS)

#include <process.h>

typedef uintptr_t fwthreadhandle;

#elif defined(FW_UNIX)

#include <pthread.h>
#include <time.h>

typedef pthread_t fwthreadhandle;

#endif

namespace Threading
{

}
