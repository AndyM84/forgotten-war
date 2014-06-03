#pragma once

#include <Common/Types.h>
#include "LogData.h"

#include <ctime>
#include <sstream>
#include <iomanip>

namespace Chimera
{
	namespace Logging
	{
		class AppenderBase
		{
		public:
			virtual cxvoid DoAppend(LogData data) = 0;
			virtual cxvoid ReOpen();
			virtual cxvoid Close();

		protected:
			AppenderBase(const cxstring name);
			~AppenderBase();
			cxstring GetTime();

			cxstring m_Name;
		};
	};
};
