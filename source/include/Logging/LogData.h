#pragma once

#include <Common/Types.h>

namespace Chimera
{
	namespace Logging
	{
		enum LogLevel
		{
			LOG_CRITICAL = 0,
			LOG_ERROR,
			LOG_DEBUG,
			LOG_WARN,
			LOG_INFO,
			LOG_TRACE
		};

		class LogData
		{
		public:
			LogData();
			LogData(LogData&& data);
			LogData(const cxstring key, const cxstring message, const LogLevel level);

			cxstring GetKey();
			cxstring GetMsg(); // There is a conflict here in WinUser.h with a #define GetMessage GetMessageA
			LogLevel GetLevel();

		protected:
			cxstring m_Key;
			cxstring m_Message;
			LogLevel m_Level;
		};
	};
};
