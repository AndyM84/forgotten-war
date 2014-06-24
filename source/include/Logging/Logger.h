#pragma once

#define FWLOG_MAX_MESSAGE_LENGTH  4096
#define FWLOG_DEFAULT_LOG_LEVEL   LOG_WARN

#include <Common/Types.h>
#include "LogData.h"
#include "AppenderBase.h"
#include "LogWorker.h"

#include <map>
#include <stdarg.h>

namespace Logging
{
	class Logger
	{
	public:
		static Logger &GetLogger(const fwstr key);

		Logger(const Logger &other);
		Logger& operator=(Logger other);
		~Logger();

		fwvoid SetDefaultLevel(const LogLevel level);
		LogLevel GetDefaultLevel();
		fwstr GetName();

		fwvoid Log(LogData *data);

		fwvoid Log(const fwchar *message);
		fwvoid Log(const fwchar *message, const LogLevel level);
		fwvoid Log(const fwstr format, ...);
		fwvoid Log(const LogLevel level, const fwstr format, ...);

		fwvoid Critical(const fwchar *message);
		fwvoid Critical(const fwstr format, ...);

		fwvoid Error(const fwchar *message);
		fwvoid Error(const fwstr format, ...);

		fwvoid Debug(const fwchar *message);
		fwvoid Debug(const fwstr format, ...);

		fwvoid Warn(const fwchar *message);
		fwvoid Warn(const fwstr format, ...);

		fwvoid Info(const fwchar *message);
		fwvoid Info(const fwstr format, ...);

		fwvoid Trace(const fwchar *message);
		fwvoid Trace(const fwstr format, ...);

	protected:
		Logger(const fwstr key);
		Logger(const fwstr key, LogLevel level);

	private:
		typedef std::map<fwstr, Logger*> LoggerMap;
		typedef std::pair<fwstr, Logger*> LoggerPair;

		static LoggerMap *allLoggers;
		static LogWorker *worker;

		LogLevel m_DefaultLevel;
		fwstr m_Name;
	};
};