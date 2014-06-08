#include "Threading/LockCriticalSection.h"

namespace Chimera
{
	namespace Threading
	{
		LockCriticalSection::LockCriticalSection()
		{
			this->m_Blocked = false;
			this->m_Error = false;

#if defined(FW_WINDOWS)
			InitializeCriticalSection(&this->m_Critsec);
#elif defined(FW_UNIX)
			pthread_mutexattr_t mAttr;
			pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE_NP);

			pthread_mutex_init(&this->m_Critsec, &mAttr);
			
			pthread_mutexattr_destroy(&mAttr)
#endif

			return;
		}

		LockCriticalSection::~LockCriticalSection()
		{
			if (!this->m_Error)
			{
#if defined(FW_WINDOWS)
				DeleteCriticalSection(&this->m_Critsec);
#elif defined(FW_UNIX)
				pthread_mutex_destroy(this->m_Critsec);
#endif
			}

			return;
		}

		fwbool LockCriticalSection::Block()
		{
			if (this->m_Error)
			{
				return false;
			}

#if defined(FW_WINDOWS)
			// Can raise an exception on timeout, but MSFT suggests not handling and instead debugging
			// http://msdn.microsoft.com/en-us/library/windows/desktop/ms682608(v=vs.85).aspx
			EnterCriticalSection(&this->m_Critsec);

			this->m_Blocked = true;

			return true;
#elif defined(FW_UNIX)
			if (pthread_mutex_lock(&this->m_Critsec) != 0)
			{
				this->isError = true;

				return(false);
			}

			this->m_Blocked = true;

			return(true);
#endif
		}

		fwbool LockCriticalSection::Block(fwword timeout)
		{
			// Not implemented, maybe not possible
			return false;
		}

		fwvoid LockCriticalSection::Release()
		{
			if (this->m_Error || !this->m_Blocked)
			{
				return;
			}

#if defined(FW_WINDOWS)
			LeaveCriticalSection(&this->m_Critsec);

			this->m_Blocked = false;
#elif defined(FW_UNIX)
			if (pthread_mutex_unlock(&this->m_Critsec) == 0)
			{
				this->m_Blocked = false;
			}
			else
			{
				this->m_Error = true;
			}
#endif

			return;
		}
	};
};
