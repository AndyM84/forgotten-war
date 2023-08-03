#include <Logging/AppenderBase.h>

namespace Logging
{
	fwvoid AppenderBase::ReOpen() { }
	fwvoid AppenderBase::Close() { }

	AppenderBase::AppenderBase(const fwstr name)
	{
		this->m_Name = name;

		return;
	}

	AppenderBase::~AppenderBase()
	{
		return;
	}

	fwstr AppenderBase::GetTime()
	{
		time_t t = time(0);
#if defined(FW_WINDOWS)
		struct std::tm *now, tmp;
		localtime_s(&tmp, &t);
		now = &tmp;
#elif defined(FW_UNIX)
		struct std::tm *now = localtime(&t);
#endif
		std::stringstream ss;

		ss << std::setfill('0') << std::setw(2) << now->tm_mday << "/"
			<< std::setfill('0') << std::setw(2) << now->tm_mon + 1 << "/"
			<< std::setfill('0') << std::setw(2) << now->tm_year + 1900 << " "
			<< std::setfill('0') << std::setw(2) << now->tm_hour << ":"
			<< std::setfill('0') << std::setw(2) << now->tm_min << ":"
			<< std::setfill('0') << std::setw(2) << now->tm_sec << " ";
		fwstr retVal = ss.str();

		return retVal;
	}
};
