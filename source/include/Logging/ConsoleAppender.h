#pragma once

#include <Logging/AppenderBase.h>

#include <ctime>
#include <string>
#include <sstream>
#include <iostream>

#if defined(FW_WINDOWS)
	#include <Windows.h>
#endif

enum CONSOLE_COLORS
{
	CONSOLE_COLOR_BLACK,
	CONSOLE_COLOR_GRAY,
	CONSOLE_COLOR_RED,
	CONSOLE_COLOR_YELLOW,
	CONSOLE_COLOR_WHITE,
	CONSOLE_COLOR_MAGENTA,
	CONSOLE_COLOR_CYAN
};

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
