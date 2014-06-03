#include <Logging/ConsoleAppender.h>

namespace Chimera
{
	namespace Logging
	{
		ConsoleAppender::ConsoleAppender(const cxstring name)
			: AppenderBase(name)
		{ }

		ConsoleAppender::ConsoleAppender(const ConsoleAppender &other)
			: AppenderBase(other.m_Name)
		{ }

		ConsoleAppender& ConsoleAppender::operator=(ConsoleAppender other)
		{
			this->m_Name = other.m_Name;

			return *this;
		}

		ConsoleAppender::~ConsoleAppender()
		{
			return;
		}

		cxvoid ConsoleAppender::DoAppend(LogData data)
		{
			std::stringstream ss;

			switch (data.GetLevel())
			{
				case LOG_CRITICAL:
					ss << "CRITICAL [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
					this->ColoredOutput(ss.str(),  CONSOLE_COLOR_MAGENTA);

					break;
				case LOG_INFO:
					ss << "INFO [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
					this->ColoredOutput(ss.str(), CONSOLE_COLOR_WHITE);

					break;
				case LOG_WARN:
					ss << "WARN [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
					this->ColoredOutput(ss.str(), CONSOLE_COLOR_YELLOW);

					break;
				case LOG_DEBUG:
					ss << "DEBUG [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
					this->ColoredOutput(ss.str(), CONSOLE_COLOR_CYAN);

					break;
				case LOG_TRACE:
					ss << "TRACE [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
					this->ColoredOutput(ss.str(), CONSOLE_COLOR_GRAY);

					break;
				case LOG_ERROR:
					ss << "ERROR [" << data.GetKey() << "] " << data.GetMsg() << std::endl;
					this->ColoredOutput(ss.str(), CONSOLE_COLOR_RED);

					break;
			}

			return;
		}

		cxvoid ConsoleAppender::ColoredOutput(cxstring text, CONSOLE_COLORS foreground)
		{
			/*
			enum {
			BLACK             = 0,
			DARKBLUE          = FOREGROUND_BLUE,
			DARKGREEN         = FOREGROUND_GREEN,
			DARKCYAN          = FOREGROUND_GREEN | FOREGROUND_BLUE,
			DARKRED           = FOREGROUND_RED,
			DARKMAGENTA       = FOREGROUND_RED | FOREGROUND_BLUE,
			DARKYELLOW        = FOREGROUND_RED | FOREGROUND_GREEN,
			DARKGRAY          = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			GRAY              = FOREGROUND_INTENSITY,
			BLUE              = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
			GREEN             = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
			CYAN              = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
			RED               = FOREGROUND_INTENSITY | FOREGROUND_RED,
			MAGENTA           = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
			YELLOW            = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
			WHITE             = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			};
			*/
			switch (foreground)
			{
			case CONSOLE_COLOR_GRAY:
#if defined CHIMERA_WINDOWS
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
				std::cout << this->GetTime() << text;
#else
				std::cout << "\033[39m" << this->GetTime() << text;
#endif

				break;

			case CONSOLE_COLOR_RED:
#if defined CHIMERA_WINDOWS
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				std::cout << this->GetTime() << text;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
#else
				std::cout << "\033[31m" << this->GetTime() << text << "\033[39m";
#endif

				break;

			case CONSOLE_COLOR_YELLOW:
#if defined CHIMERA_WINDOWS
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				std::cout << this->GetTime() << text;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
#else
				std::cout << "\033[33m" << this->GetTime() << text << "\033[39m";
#endif

				break;

			case CONSOLE_COLOR_WHITE:
#if defined CHIMERA_WINDOWS
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				std::cout << this->GetTime() << text;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
#else
				std::cout << "\033[97m" << this->GetTime() << text << "\033[39m";
#endif

				break;

			case CONSOLE_COLOR_MAGENTA:
#if defined CHIMERA_WINDOWS
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				std::cout << this->GetTime() << text;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
#else
				std::cout << "\033[35m" << this->GetTime() << text << "\033[39m";
#endif

				break;

			case CONSOLE_COLOR_CYAN:
#if defined CHIMERA_WINDOWS
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
				std::cout << this->GetTime() << text;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
#else
				std::cout << "\033[35m" << this->GetTime() << text << "\033[39m";
#endif

				break;
			}
			
			return;
		}
	};
};
