#pragma once

#include <Common/Types.h>

#define FW_INIT_LIBRARY(name) \
extern "C" { \
	FW_LIB_EXPORT name *InitLibrary(); \
	FW_LIB_EXPORT name *InitLibrary() \
	{ \
		return new name; \
	} \
}

namespace Libraries
{
	class FW_LIB_EXPORT Library
	{
	public:
		virtual fwbool Setup() = 0;
		virtual fwbool Destroy() = 0;
	};
}
