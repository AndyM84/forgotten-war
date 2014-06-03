#include <Logging/AppenderBase.h>

namespace Chimera
{
	namespace Logging
	{
		cxvoid AppenderBase::ReOpen() { }
		cxvoid AppenderBase::Close() { }

		AppenderBase::AppenderBase(const cxstring name)
		{
			this->m_Name = name;

			return;
		}

		AppenderBase::~AppenderBase()
		{
			return;
		}

		cxstring AppenderBase::GetTime()
		{
			time_t t = time(0);
			struct std::tm* now = localtime(&t);
			std::stringstream ss;

			ss << std::setfill('0') << std::setw(2) << now->tm_mday << "/"
				 << std::setfill('0') << std::setw(2) << now->tm_mon + 1 << "/"
				 << std::setfill('0') << std::setw(2) << now->tm_year + 1900 << " "
				 << std::setfill('0') << std::setw(2) << now->tm_hour << ":"
				 << std::setfill('0') << std::setw(2) << now->tm_min << ":"
				 << std::setfill('0') << std::setw(2) << now->tm_sec << " ";
			std::string retVal = ss.str();

			return retVal;
		}
	};
};
