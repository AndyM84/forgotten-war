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
			~Logger();

			void SetDefaultLevel(const LogLevel level);

			void Log(LogData *data);

			void Log(const cxchar *message);
			void Log(const cxchar *message, const LogLevel level);
			void Log(const cxstring format, ...);
			void Log(const LogLevel level, const cxstring format, ...);

			void Critical(const cxchar *message);
			void Critical(const cxstring format, ...);

			void Error(const cxchar *message);
			void Error(const cxstring format, ...);

			void Debug(const cxchar *message);
			void Debug(const cxstring format, ...);

			void Warn(const cxchar *message);
			void Warn(const cxstring format, ...);

			void Info(const cxchar *message);
			void Info(const cxstring format, ...);

			void Trace(const cxchar *message);
			void Trace(const cxstring format, ...);

		protected:
			Logger(const cxstring key);
			Logger(const cxstring key, LogLevel level);

		private:
			typedef std::map<cxstring, Logger*> LoggerMap;
			typedef std::pair<cxstring, Logger*> LoggerPair;

			static LoggerMap *allLoggers;
			static LogWorker *worker;

			const cxstring m_Name;

			LogLevel m_DefaultLevel;
		};
	};
};
