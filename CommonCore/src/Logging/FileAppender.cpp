#include <Logging/FileAppender.h>

namespace Logging
{
	FileAppender::FileAppender(const fwstr name)
		: AppenderBase(name)
	{
		this->m_Filename.append(name);
		this->m_Filename.append(".log");

		this->Open();

		return;
	}

	FileAppender::FileAppender(const fwstr name, const fwstr file)
		: AppenderBase(name)
	{
		this->m_Filename = file;

		this->Open();

		return;
	}

	FileAppender::FileAppender(const FileAppender &other)
		: AppenderBase(other.m_Name)
	{
		this->m_Filename = other.m_Filename;

		this->Open();

		return;
	}

	FileAppender &FileAppender::operator=(FileAppender other)
	{
		this->m_Name = other.m_Name;
		this->m_Filename = other.m_Filename;

		this->ReOpen();

		return *this;
	}

	FileAppender::~FileAppender()
	{
		this->Close();

		return;
	}

	fwvoid FileAppender::DoAppend(LogData data)
	{
		std::stringstream ss;

		switch (data.GetLevel()) {
		case LOG_CRITICAL:
			ss << "CRITICAL [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
			this->OutputToFile(ss.str());

			break;
		case LOG_INFO:
			ss << "INFO [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
			this->OutputToFile(ss.str());

			break;
		case LOG_WARN:
			ss << "WARN [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
			this->OutputToFile(ss.str());

			break;
		case LOG_DEBUG:
			ss << "DEBUG [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
			this->OutputToFile(ss.str());

			break;
		case LOG_TRACE:
			ss << "TRACE [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
			this->OutputToFile(ss.str());

			break;
		case LOG_ERROR:
			ss << "ERROR [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
			this->OutputToFile(ss.str());

			break;
		}

		return;
	}

	fwvoid FileAppender::ReOpen()
	{
		if (this->m_LogFile.is_open()) {
			this->Close();
		}

		this->Open();

		return;
	}

	fwvoid FileAppender::Close()
	{
		if (this->m_LogFile.is_open()) {
			this->m_LogFile.close();
		}

		return;
	}

	fwvoid FileAppender::Open()
	{
		if (this->m_LogFile.is_open()) {
			return;
		}

		this->m_LogFile.open(this->m_Filename.c_str(), std::ios::out | std::ios::trunc);

		return;
	}

	fwvoid FileAppender::OutputToFile(fwstr line)
	{
		if (!this->m_LogFile.is_open()) {
			return;
		}

		this->m_LogFile << this->GetTime() << line;
		std::flush(this->m_LogFile);

		return;
	}
};
