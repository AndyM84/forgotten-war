#include "Logging/LogData.h"

namespace Chimera
{
	namespace Logging
	{
		LogData::LogData()
		{
			m_Key = "";
			m_Message = "";
			m_Level = LOG_TRACE;

			return;
		}

		LogData::LogData(LogData&& data)
			: m_Key(data.GetKey()), m_Message(data.GetMsg()), m_Level(data.GetLevel())
		{ }

		LogData::LogData(const cxstring key, const cxstring message, const LogLevel level)
			: m_Key(key), m_Message(message), m_Level(level)
		{ }

		cxstring LogData::GetKey()
		{
			return this->m_Key;
		}

		cxstring LogData::GetMsg()
		{
			return this->m_Message;
		}

		LogLevel LogData::GetLevel()
		{
			return this->m_Level;
		}
	};
};
