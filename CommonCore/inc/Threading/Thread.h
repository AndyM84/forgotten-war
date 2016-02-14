#pragma once

#include <Common/Types.h>
#include <Threading/Threadable.h>

typedef void* fwlptrvoid;

#if defined(FW_WINDOWS)
	#include <Windows.h>
	#include <process.h>

	typedef uintptr_t fwthreadhandle;
#elif defined(FW_UNIX)
	#include <pthread.h>
	#include <time.h>

	typedef pthread_t fwthreadhandle;
#endif

enum THREAD_STATUS
{
	THREAD_UNKNOWN,
	THREAD_READY,
	THREAD_RUNNING,
	THREAD_PAUSED,
	THREAD_TERMINATED
};

namespace Threading
{
	class Thread
	{
	public:
		~Thread();
		Thread(Threadable &target);

		fwvoid Start();
		fwvoid Terminate();
		fwvoid CloseThread();
		THREAD_STATUS GetStatus();

	protected:
		THREAD_STATUS m_ThreadStatus;

	private:
		Thread();
		FW_LIB_STDCALL StaticThreadEntry(fwvoid *param);

		Threadable *m_Target;
		fwthreadhandle m_Handle;
	};
}
