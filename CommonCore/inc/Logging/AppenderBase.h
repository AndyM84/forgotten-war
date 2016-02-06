#pragma once

#include <Common/Types.h>
#include "LogData.h"

#include <ctime>
#include <sstream>
#include <iomanip>

namespace Logging
{
	class AppenderBase
	{
	public:
		virtual fwvoid DoAppend(LogData data) = 0;
		virtual fwvoid ReOpen();
		virtual fwvoid Close();

	protected:
		AppenderBase(const fwstr name);
		~AppenderBase();
		fwstr GetTime();

		fwstr m_Name;
	};
};
