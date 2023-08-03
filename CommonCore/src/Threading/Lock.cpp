#include "Threading/Lock.h"

namespace Threading
{
	fwbool Lock::IsBlocked()
	{
		if (this->m_Error) {
			return false;
		}

		return this->m_Blocked;
	}

	fwbool Lock::IsError()
	{
		return this->m_Error;
	}
};
