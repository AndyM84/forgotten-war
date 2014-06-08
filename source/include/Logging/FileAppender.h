#pragma once

#include <Logging/AppenderBase.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#if defined(FW_WINDOWS)
#include <Windows.h>
#endif

namespace Logging
{
	class FileAppender : public AppenderBase
	{
	public:
		FileAppender(const fwstr name);
		FileAppender(const fwstr name, const fwstr file);
		FileAppender(const FileAppender &other);
		FileAppender& operator=(FileAppender other);
		~FileAppender();

		virtual fwvoid DoAppend(LogData data);
		virtual fwvoid ReOpen();
		virtual fwvoid Close();

	protected:
		fwvoid Open();
		fwvoid OutputToFile(fwstr line);

		std::ofstream m_LogFile;
		fwstr m_Filename;
	};
};
