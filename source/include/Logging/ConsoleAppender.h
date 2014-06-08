#pragma once

#include <Logging/AppenderBase.h>

#include <ctime>
#include <string>
#include <sstream>
#include <iostream>

#if defined(FW_WINDOWS)
#include <Windows.h>
#endif

namespace Logging
{
	class ConsoleAppender : public AppenderBase
	{
	public:
		ConsoleAppender(const fwstr name);
		ConsoleAppender(const ConsoleAppender &other);
		ConsoleAppender& operator=(ConsoleAppender other);
		~ConsoleAppender();

		virtual fwvoid DoAppend(LogData data);

	protected:
		fwvoid ColoredOutput(fwstr text, CONSOLE_COLORS foreground);
	};
};
