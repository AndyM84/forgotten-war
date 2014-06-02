#include "Logging/Logger.h"

namespace Chimera
{
	namespace Logging
	{
		Logger::LoggerMap *Logger::allLoggers = 0;
		LogWorker *Logger::worker = 0;

		/* PUBLIC */

		Logger &Logger::GetLogger(const cxstring key)
		{
			if (!allLoggers)
			{
				allLoggers = new LoggerMap();
			}

			LoggerMap::iterator logger = allLoggers->find(key);

			if (logger == allLoggers->end())
			{
				allLoggers->insert(LoggerPair(key, new Logger(key)));
				logger = allLoggers->find(key);
			}

			return *logger->second;
		}

		Logger::~Logger()
		{
			return;
		}

		void Logger::SetDefaultLevel(const LogLevel level)
		{
			this->m_DefaultLevel = level;

			return;
		}

		void Logger::Log(LogData *data)
		{
			LogWorker::AddMessage(data);

			return;
		}

		void Logger::Log(const cxchar *message)
		{
			this->Log(new LogData(this->m_Name, message, this->m_DefaultLevel));

			return;
		}

		void Logger::Log(const cxchar *message, const LogLevel level)
		{
			this->Log(new LogData(this->m_Name, message, level));

			return;
		}

		void Logger::Log(const cxstring format, ...)
		{
			char msg[CXLOG_MAX_MESSAGE_LENGTH];

			va_list argptr;
			va_start(argptr, format);
			vsprintf(msg, format.c_str(), argptr);
			va_end(argptr);

			this->Log(new LogData(this->m_Name, msg, this->m_DefaultLevel));

			return;
		}

		void Logger::Log(const LogLevel level, const cxstring format, ...)
		{
			char msg[CXLOG_MAX_MESSAGE_LENGTH];

			va_list argptr;
			va_start(argptr, format);
			vsprintf(msg, format.c_str(), argptr);
			va_end(argptr);

			this->Log(new LogData(this->m_Name, msg, level));

			return;
		}

		void Logger::Critical(const cxchar *message)
		{
			this->Log(message, LOG_CRITICAL);

			return;
		}

		void Logger::Critical(const cxstring format, ...)
		{
			char msg[CXLOG_MAX_MESSAGE_LENGTH];

			va_list argptr;
			va_start(argptr, format);
			vsprintf(msg, format.c_str(), argptr);
			va_end(argptr);

			this->Log(msg, LOG_CRITICAL);

			return;
		}

		void Logger::Error(const cxchar *message)
		{
			this->Log(message, LOG_ERROR);

			return;
		}

		void Logger::Error(const cxstring format, ...)
		{
			char msg[CXLOG_MAX_MESSAGE_LENGTH];

			va_list argptr;
			va_start(argptr, format);
			vsprintf(msg, format.c_str(), argptr);
			va_end(argptr);

			this->Log(msg, LOG_ERROR);

			return;
		}

		void Logger::Debug(const cxchar *message)
		{
			this->Log(message, LOG_DEBUG);

			return;
		}

		void Logger::Debug(const cxstring format, ...)
		{
			char msg[CXLOG_MAX_MESSAGE_LENGTH];

			va_list argptr;
			va_start(argptr, format);
			vsprintf(msg, format.c_str(), argptr);
			va_end(argptr);

			this->Log(msg, LOG_DEBUG);

			return;
		}

		void Logger::Warn(const cxchar *message)
		{
			this->Log(message, LOG_WARN);

			return;
		}

		void Logger::Warn(const cxstring format, ...)
		{
			char msg[CXLOG_MAX_MESSAGE_LENGTH];

			va_list argptr;
			va_start(argptr, format);
			vsprintf(msg, format.c_str(), argptr);
			va_end(argptr);

			this->Log(msg, LOG_WARN);

			return;
		}

		void Logger::Info(const cxchar *message)
		{
			this->Log(message, LOG_INFO);

			return;
		}

		void Logger::Info(const cxstring format, ...)
		{
			char msg[CXLOG_MAX_MESSAGE_LENGTH];

			va_list argptr;
			va_start(argptr, format);
			vsprintf(msg, format.c_str(), argptr);
			va_end(argptr);

			this->Log(msg, LOG_INFO);

			return;
		}

		void Logger::Trace(const cxchar *message)
		{
			this->Log(message, LOG_TRACE);

			return;
		}

		void Logger::Trace(const cxstring format, ...)
		{
			char msg[CXLOG_MAX_MESSAGE_LENGTH];

			va_list argptr;
			va_start(argptr, format);
			vsprintf(msg, format.c_str(), argptr);
			va_end(argptr);

			this->Log(msg, LOG_TRACE);

			return;
		}

		/* /PUBLIC */
		/* PROTECTED */

		Logger::Logger(const cxstring key) : m_Name(key)
		{
			this->m_DefaultLevel = CXLOG_DEFAULT_LOG_LEVEL;

			return;
		}

		Logger::Logger(const cxstring key, LogLevel level) : m_Name(key)
		{
			this->m_DefaultLevel = level;
		}

		/* /PROTECTED */
	};
};
