#include "Threading/Lock.h"

namespace Chimera
{
	namespace Threading
	{
		cxbool Lock::IsBlocked()
		{
			if (this->m_Error)
			{
				return false;
			}

			return this->m_Blocked;
		}

		cxbool Lock::IsError()
		{
			return this->m_Error;
		}
	};
};
