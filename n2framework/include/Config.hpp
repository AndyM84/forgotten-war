#pragma once

enum N2F_LOG_LEVELS
{
	N2F_LOG_NONE = 0,
	N2F_LOG_DEBUG = 1,
	N2F_LOG_INFO = 2,
	N2F_LOG_NOTICE = 4,
	N2F_LOG_WARNING = 8,
	N2F_LOG_ERROR = 16,
	N2F_LOG_CRITICAL = 32,
	N2F_LOG_ALERT = 64,
	N2F_LOG_EMERGENCY = 128,
	N2F_LOG_ALL = 255
};

namespace N2f
{
	class Config
	{

	};

	class LoggerConfig
	{
	protected:
		int _logLevel = N2F_LOG_NONE;
		bool _dumpLogs = false;

	public:
		LoggerConfig();
		LoggerConfig(bool DumpLogs, int LogLevel);

		const bool ShouldDumpLogs();
		const int LogLevel();
	};
}
