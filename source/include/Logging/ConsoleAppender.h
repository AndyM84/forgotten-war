#pragma once

#include <Logging/AppenderBase.h>

#include <ctime>
#include <string>
#include <sstream>
#include <iostream>

#ifdef CHIMERA_WINDOWS
#include <Windows.h>
#endif

namespace Chimera
{
	namespace Logging
	{
		class ConsoleAppender : public AppenderBase
		{
		public:
			ConsoleAppender(const cxstring name);
			ConsoleAppender(const ConsoleAppender &other);
			ConsoleAppender& operator=(ConsoleAppender other);
			~ConsoleAppender();

			virtual cxvoid DoAppend(LogData data);

		protected:
			cxvoid ColoredOutput(cxstring text, CONSOLE_COLORS foreground);
		};
	};
};
