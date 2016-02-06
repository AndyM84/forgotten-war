#pragma once

#include <Common/Types.h>

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
		LogData(const fwstr key, const fwstr message, const LogLevel level);
		LogData(const LogData &other);
		LogData& operator=(LogData other);
		~LogData();

		fwstr GetKey();
		fwstr GetMsg(); // There is a conflict here in WinUser.h with a #define GetMessage GetMessageA
		LogLevel GetLevel();

	protected:
		fwstr m_Key;
		fwstr m_Message;
		LogLevel m_Level;
	};
};
