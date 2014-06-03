#pragma once

#define CXLOG_MAX_MESSAGE_LENGTH	4096
#define CXLOG_DEFAULT_LOG_LEVEL		LOG_WARN

#include <Common/Types.h>
#include "LogData.h"
#include "AppenderBase.h"
#include "LogWorker.h"

#include <map>
#include <stdarg.h>

namespace Chimera
{
	namespace Logging
	{
		class Logger
		{
		public:
			static Logger &GetLogger(const cxstring key);

			Logger(const Logger &other);
			Logger& operator=(Logger other);
			~Logger();

			cxvoid SetDefaultLevel(const LogLevel level);
			LogLevel GetDefaultLevel();
			cxstring GetName();

			cxvoid Log(LogData *data);

			cxvoid Log(const cxchar *message);
			cxvoid Log(const cxchar *message, const LogLevel level);
			cxvoid Log(const cxstring format, ...);
			cxvoid Log(const LogLevel level, const cxstring format, ...);

			cxvoid Critical(const cxchar *message);
			cxvoid Critical(const cxstring format, ...);

			cxvoid Error(const cxchar *message);
			cxvoid Error(const cxstring format, ...);

			cxvoid Debug(const cxchar *message);
			cxvoid Debug(const cxstring format, ...);

			cxvoid Warn(const cxchar *message);
			cxvoid Warn(const cxstring format, ...);

			cxvoid Info(const cxchar *message);
			cxvoid Info(const cxstring format, ...);

			cxvoid Trace(const cxchar *message);
			cxvoid Trace(const cxstring format, ...);

		protected:
			Logger(const cxstring key);
			Logger(const cxstring key, LogLevel level);

		private:
			typedef std::map<cxstring, Logger*> LoggerMap;
			typedef std::pair<cxstring, Logger*> LoggerPair;

			static LoggerMap *allLoggers;
			static LogWorker *worker;

			LogLevel m_DefaultLevel;
			cxstring m_Name;
		};
	};
};
