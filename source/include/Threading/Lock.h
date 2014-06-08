#pragma once

#include <Common/Types.h>
#include <Threading/Thread.h>

namespace Threading
{
	class Lock
	{
	public:
		virtual fwbool Block() = 0;
		virtual fwbool Block(fwword timeout) = 0;
		virtual fwvoid Release() = 0;

		fwbool IsBlocked();
		fwbool IsError();

	protected:
		fwbool m_Blocked, m_Error;
	};
};
