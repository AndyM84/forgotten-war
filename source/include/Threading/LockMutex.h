#pragma once

#include <Threading/Lock.h>

#if defined CHIMERA_WINDOWS
typedef HANDLE cxmutex;
#elif defined CHIMERA_UNIX
typedef pthread_mutex_t cxmutex;
#elif defined CHIMERA_ORBIS
typedef pthread_mutex_t cxmutex;
#endif

namespace Chimera
{
	namespace Threading
	{
		class LockMutex : public Lock
		{
		public:
			LockMutex();
			~LockMutex();

			virtual cxbool Block();
			virtual cxbool Block(cxword timeout);
			virtual cxvoid Release();

		protected:
			cxmutex m_Mutex;
		};
	};
};
