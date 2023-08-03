#include <Threading/Thread.h>

namespace Threading
{
	Thread::~Thread()
	{
		this->CloseThread();

		return;
	}

	Thread::Thread(Threadable &target)
	{
		this->m_Target = &target;
		this->m_ThreadStatus = THREAD_READY;

#ifdef FW_WINDOWS
		this->m_Handle = _beginthreadex(NULL, 0, &Thread::StaticThreadEntry, this, CREATE_SUSPENDED, NULL);
#endif

		return;
	}

	FW_LIB Thread::StaticThreadEntry(void *param)
	{
		Thread *ptr = reinterpret_cast<Thread *>(param);

		if (ptr->m_Target && ptr->m_Target->IsValid()) {
			ptr->m_Target->Run();
		}

		return 0;
	}

	fwvoid Thread::Start()
	{
		this->m_ThreadStatus = THREAD_RUNNING;

#ifdef FW_WINDOWS
		ResumeThread((HANDLE)this->m_Handle);
#else
		pthread_create(&this->m_Handle, NULL, StaticThreadEntry, (void *)this);
#endif

		return;
	}

	fwvoid Thread::Terminate()
	{
		this->m_ThreadStatus = THREAD_TERMINATED;

		if (this->m_Target && this->m_Handle != NULL) {
			this->m_Target->SignalTerminate();
		}

		return;
	}

	fwvoid Thread::CloseThread()
	{
		if (this->m_Handle == NULL) {
			return;
		}

#ifdef FW_WINDOWS
		TerminateThread((HANDLE)this->m_Handle, 0);
#else
#endif

		this->m_Handle = NULL;

		return;
	}

	THREAD_STATUS Thread::GetStatus()
	{
		// TODO: Should we be guarding these members against race conditions?
		return this->m_ThreadStatus;
	}
}