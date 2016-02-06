#include <Logging/LogData.h>

namespace Logging
{
	LogData::LogData()
	{
		m_Key = "";
		m_Message = "";
		m_Level = LOG_TRACE;

		return;
	}

	LogData::LogData(const fwstr key, const fwstr message, const LogLevel level)
		: m_Key(key), m_Message(message), m_Level(level)
	{ }

	LogData::LogData(const LogData &other)
	{
		this->m_Key = other.m_Key;
		this->m_Message = other.m_Message;
		this->m_Level = other.m_Level;

		return;
	}

	LogData& LogData::operator=(LogData other)
	{
		this->m_Key = other.m_Key;
		this->m_Message = other.m_Message;
		this->m_Level = other.m_Level;

		return *this;
	}

	LogData::~LogData()
	{
		return;
	}

	fwstr LogData::GetKey()
	{
		return this->m_Key;
	}

	fwstr LogData::GetMsg()
	{
		return this->m_Message;
	}

	LogLevel LogData::GetLevel()
	{
		return this->m_Level;
	}
};
