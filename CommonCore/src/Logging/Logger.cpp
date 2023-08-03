#define _CRT_SECURE_NO_WARNINGS

#include <Logging/Logger.h>

namespace Logging
{
	Logger::LoggerMap *Logger::allLoggers = 0;

	/* PUBLIC */

	Logger &Logger::GetLogger(const fwstr key)
	{
		if (!allLoggers) {
			allLoggers = new LoggerMap();
		}

		LoggerMap::iterator logger = allLoggers->find(key);

		if (logger == allLoggers->end()) {
			allLoggers->insert(LoggerPair(key, new Logger(key)));
			logger = allLoggers->find(key);
		}

		return *logger->second;
	}

	Logger::~Logger()
	{
		return;
	}

	fwvoid Logger::SetReportingLevel(const int level)
	{
		this->m_ReportingLevel = level;

		return;
	}

	fwvoid Logger::SetReportingLevel(const LogLevel level)
	{
		this->m_ReportingLevel = static_cast<int>(level);

		return;
	}

	fwvoid Logger::SetDefaultLevel(const LogLevel level)
	{
		this->m_DefaultLevel = level;

		return;
	}

	LogLevel Logger::GetDefaultLevel()
	{
		return this->m_DefaultLevel;
	}

	fwstr Logger::GetName()
	{
		return this->m_Name;
	}

	fwvoid Logger::Log(LogData *data)
	{
		if (this->m_ReportingLevel & data->GetLevel()) {
			LogWorker::AddMessage(data);
		}

		return;
	}

	fwvoid Logger::Log(const fwchar *message)
	{
		this->Log(new LogData(this->m_Name, message, this->m_DefaultLevel));

		return;
	}

	fwvoid Logger::Log(const fwchar *message, const LogLevel level)
	{
		this->Log(new LogData(this->m_Name, message, level));

		return;
	}

	fwvoid Logger::Log(const fwstr format, ...)
	{
		char msg[FWLOG_MAX_MESSAGE_LENGTH];

		va_list argptr;
		va_start(argptr, format);
		vsprintf(msg, format.c_str(), argptr);
		va_end(argptr);

		this->Log(new LogData(this->m_Name, msg, this->m_DefaultLevel));

		return;
	}

	fwvoid Logger::Log(const LogLevel level, const fwstr format, ...)
	{
		char msg[FWLOG_MAX_MESSAGE_LENGTH];

		va_list argptr;
		va_start(argptr, format);
		vsprintf(msg, format.c_str(), argptr);
		va_end(argptr);

		this->Log(new LogData(this->m_Name, msg, level));

		return;
	}

	fwvoid Logger::Critical(const fwchar *message)
	{
		this->Log(message, LOG_CRITICAL);

		return;
	}

	fwvoid Logger::Critical(const fwstr format, ...)
	{
		char msg[FWLOG_MAX_MESSAGE_LENGTH];

		va_list argptr;
		va_start(argptr, format);
		vsprintf(msg, format.c_str(), argptr);
		va_end(argptr);

		this->Log(msg, LOG_CRITICAL);

		return;
	}

	fwvoid Logger::Error(const fwchar *message)
	{
		this->Log(message, LOG_ERROR);

		return;
	}

	fwvoid Logger::Error(const fwstr format, ...)
	{
		char msg[FWLOG_MAX_MESSAGE_LENGTH];

		va_list argptr;
		va_start(argptr, format);
		vsprintf(msg, format.c_str(), argptr);
		va_end(argptr);

		this->Log(msg, LOG_ERROR);

		return;
	}

	fwvoid Logger::Debug(const fwchar *message)
	{
		this->Log(message, LOG_DEBUG);

		return;
	}

	fwvoid Logger::Debug(const fwstr format, ...)
	{
		char msg[FWLOG_MAX_MESSAGE_LENGTH];

		va_list argptr;
		va_start(argptr, format);
		vsprintf(msg, format.c_str(), argptr);
		va_end(argptr);

		this->Log(msg, LOG_DEBUG);

		return;
	}

	fwvoid Logger::Warn(const fwchar *message)
	{
		this->Log(message, LOG_WARN);

		return;
	}

	fwvoid Logger::Warn(const fwstr format, ...)
	{
		char msg[FWLOG_MAX_MESSAGE_LENGTH];

		va_list argptr;
		va_start(argptr, format);
		vsprintf(msg, format.c_str(), argptr);
		va_end(argptr);

		this->Log(msg, LOG_WARN);

		return;
	}

	fwvoid Logger::Info(const fwchar *message)
	{
		this->Log(message, LOG_INFO);

		return;
	}

	fwvoid Logger::Info(const fwstr format, ...)
	{
		char msg[FWLOG_MAX_MESSAGE_LENGTH];

		va_list argptr;
		va_start(argptr, format);
		vsprintf(msg, format.c_str(), argptr);
		va_end(argptr);

		this->Log(msg, LOG_INFO);

		return;
	}

	fwvoid Logger::Trace(const fwchar *message)
	{
		this->Log(message, LOG_TRACE);

		return;
	}

	fwvoid Logger::Trace(const fwstr format, ...)
	{
		char msg[FWLOG_MAX_MESSAGE_LENGTH];

		va_list argptr;
		va_start(argptr, format);
		vsprintf(msg, format.c_str(), argptr);
		va_end(argptr);

		this->Log(msg, LOG_TRACE);

		return;
	}

	/* /PUBLIC */
	/* PROTECTED */

	Logger::Logger(const fwstr key)
		: m_Name(key)
	{
		this->m_DefaultLevel = FWLOG_DEFAULT_LOG_LEVEL;
		this->m_ReportingLevel = FWLOG_DEFAULT_REPORTING_LEVEL;

		return;
	}

	Logger::Logger(const fwstr key, LogLevel level)
		: m_Name(key)
	{
		this->m_DefaultLevel = level;
		this->m_ReportingLevel = FWLOG_DEFAULT_REPORTING_LEVEL;

		return;
	}

	Logger::Logger(const fwstr key, LogLevel level, LogLevel reporting)
		: m_Name(key)
	{
		this->m_DefaultLevel = level;
		this->m_ReportingLevel = reporting;

		return;
	}

	/* /PROTECTED */
};
