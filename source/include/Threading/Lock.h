#pragma once

#include <Common/Types.h>
#include <Threading/Thread.h>

namespace Chimera
{
	namespace Threading
	{
		class Lock
		{
		public:
			virtual cxbool Block() = 0;
			virtual cxbool Block(cxword timeout) = 0;
			virtual cxvoid Release() = 0;

			cxbool IsBlocked();
			cxbool IsError();

		protected:
			cxbool m_Blocked, m_Error;
		};
	};
};
