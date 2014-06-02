#include "Threading/LockMutex.h"

namespace Chimera
{
	namespace Threading
	{
		LockMutex::LockMutex()
		{
			this->m_Blocked = false;
			this->m_Error = false;

#if defined CHIMERA_WINDOWS
			this->m_Mutex = CreateMutex(NULL, false, NULL);

			if (this->m_Mutex == NULL)
			{
				this->m_Error = true;
			}
#elif defined CHIMERA_LINUX
			if (pthread_mutex_init(&this->m_Mutex, NULL) != 0)
			{
				this->m_Error = true;
			}
#elif defined CHIMERA_ORBIS
			if (pthread_mutex_init(&this->m_Mutex, NULL) != 0)
			{
				this->m_Error = true;
			}
#endif

			return;
		}

		LockMutex::~LockMutex()
		{
			if (!this->m_Error)
			{
#if defined CHIMERA_WINDOWS
				CloseHandle(this->m_Mutex);
#elif defined CHIMERA_LINUX
				pthread_mutex_destroy(this->m_Mutex);
#elif defined CHIMERA_ORBIS
				pthread_mutex_destroy(this->m_Mutex);
#endif
			}

			return;
		}

		cxbool LockMutex::Block()
		{
			if (this->m_Error)
			{
				return false;
			}

#if defined CHIMERA_WINDOWS
			DWORD res = WaitForSingleObject(this->m_Mutex, INFINITE);

			if (res != WAIT_OBJECT_0)
			{
				this->m_Error = true;

				return false;
			}

			this->m_Blocked = true;

			return true;
#elif defined CHIMERA_LINUX
			if (pthread_mutex_lock(&this->m_Mutex) != 0)
			{
				this->isError = true;

				return(false);
			}

			this->m_Blocked = true;

			return(true);
#elif defined CHIMERA_ORBIS
			if (pthread_mutex_lock(&this->m_Mutex) != 0)
			{
				this->isError = true;

				return(false);
			}

			this->m_Blocked = true;

			return(true);
#endif
		}

		cxbool LockMutex::Block(cxword timeout)
		{
			if (this->m_Error)
			{
				return false;
			}

#if defined CHIMERA_WINDOWS
			DWORD res = WaitForSingleObject(this->m_Mutex, timeout);

			if (res != WAIT_OBJECT_0)
			{
				this->m_Error = true;

				return(false);
			}

			this->m_Blocked = true;

			return(true);
#elif defined CHIMERA_LINUX
			struct timespec abs_time;

			clock_gettime(CLOCK_REALTIME, &abs_time);
			abs_time.tv_nsec += timeout;

			if (pthread_mutex_timedlock(&this->m_Mutex, &abs_time) != 0)
			{
				this->m_Error = true;

				return(false);
			}

			this->m_Blocked = true;

			return(true);
#elif defined CHIMERA_ORBIS
			struct timespec abs_time;

			clock_gettime(CLOCK_REALTIME, &abs_time);
			abs_time.tv_nsec += timeout;

			if (pthread_mutex_timedlock(&this->m_Mutex, &abs_time) != 0)
			{
				this->m_Error = true;

				return(false);
			}

			this->m_Blocked = true;

			return(true);
#endif
		}

		cxvoid LockMutex::Release()
		{
			if (this->m_Error || !this->m_Blocked)
			{
				return;
			}

#if defined CHIMERA_WINDOWS
			if (ReleaseMutex(this->m_Mutex))
			{
				this->m_Blocked = false;
			}
			else
			{
				this->m_Error = true;
			}
#elif defined CHIMERA_LINUX
			if (pthread_mutex_unlock(&this->m_Mutex) == 0)
			{
				this->m_Blocked = false;
			}
			else
			{
				this->m_Error = true;
			}
#elif defined CHIMERA_ORBIS
			if (pthread_mutex_unlock(&this->m_Mutex) == 0)
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
