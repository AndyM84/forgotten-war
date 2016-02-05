#pragma once

#include <Threading/Lock.h>

#if defined(FW_WINDOWS)
	typedef HANDLE fwmutex;
#elif defined(FW_UNIX)
	typedef pthread_mutex_t fwmutex;
#endif

namespace Threading
{
	class LockMutex : public Lock
	{
	public:
		LockMutex();
		~LockMutex();

		virtual fwbool Block();
		virtual fwbool Block(fwword timeout);
		virtual fwvoid Release();

	protected:
		fwmutex m_Mutex;
	};
};
