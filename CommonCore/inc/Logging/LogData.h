#pragma once

#include <Common/Types.h>

namespace Logging
{
	enum LogLevel
	{
		LOG_CRITICAL = 1,
		LOG_ERROR = 2,
		LOG_DEBUG = 4,
		LOG_WARN = 8,
		LOG_INFO = 16,
		LOG_TRACE = 32,
		LOG_ALL = 63
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
