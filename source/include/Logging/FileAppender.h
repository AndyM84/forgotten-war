#pragma once

#include <Logging/AppenderBase.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#ifdef CHIMERA_WINDOWS
#include <Windows.h>
#endif

namespace Chimera
{
	namespace Logging
	{
		class FileAppender : public AppenderBase
		{
		public:
			FileAppender(const cxstring name);
			FileAppender(const cxstring name, const cxstring file);
			FileAppender(const FileAppender &other);
			FileAppender& operator=(FileAppender other);
			~FileAppender();

			virtual cxvoid DoAppend(LogData data);
			virtual cxvoid ReOpen();
			virtual cxvoid Close();

		protected:
			cxvoid Open();
			cxvoid OutputToFile(cxstring line);

			std::ofstream m_LogFile;
			cxstring m_Filename;
		};
	};
};
