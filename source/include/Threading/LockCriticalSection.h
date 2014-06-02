#pragma once

#include <Threading/Lock.h>

#if defined CHIMERA_WINDOWS
typedef CRITICAL_SECTION cxcritsec;
#elif defined CHIMERA_UNIX
typedef pthread_mutex_t cxcritsec;
#elif defined CHIMERA_ORBIS
typedef pthread_mutex_t cxcritsec;
#endif

namespace Chimera
{
	namespace Threading
	{
		class LockCriticalSection : public Lock
		{
		public:
			LockCriticalSection();
			~LockCriticalSection();

			virtual cxbool Block();
			virtual cxbool Block(cxword timeout);
			virtual cxvoid Release();

		protected:
			cxcritsec m_Critsec;
		};
	};
};
