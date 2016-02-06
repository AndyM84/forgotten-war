#pragma once

#include <Threading/Lock.h>

#if defined(FW_WINDOWS)
	typedef CRITICAL_SECTION fwcritsec;
#elif defined(FW_UNIX)
	typedef pthread_mutex_t fwcritsec;
#endif

namespace Threading
{
	class LockCriticalSection : public Lock
	{
	public:
		LockCriticalSection();
		~LockCriticalSection();

		virtual fwbool Block();
		virtual fwbool Block(fwword timeout);
		virtual fwvoid Release();

	protected:
		fwcritsec m_Critsec;
	};
};
