#include <ForgottenWar.h>

// https://msdn.microsoft.com/en-us/library/bb540475%28v=vs.85%29.aspx

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#pragma comment(lib, "advapi32.lib")

#define SVCNAME TEXT("ForgottenWar")

int RunFW(int port);
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);

int main(int argc, char *argv[])
{
	if (argc > 0)
	{
		auto disp = CliDispatch();
		int result = 0;

		disp.Initialize(argc, argv);
		std::cout << "There were " << disp.NumResults() << " arguments." << std::endl;

		if (disp.NumResults() == 1)
		{
			RunFW(9005);
		}
		else
		{
			if (disp.NumResults() == 2)
			{
				auto args = disp.GetRawParameters();

				result = RunFW(atoi(args[1].c_str()));
			}

			// TODO: Add some more stuff here, like perhaps look into log levels and what not
		}

		return result;
	}
	else
	{
		SERVICE_TABLE_ENTRY sTable[] = {
			{ _T(""), &ServiceMain },
			{ NULL, NULL }
		};

		if (StartServiceCtrlDispatcher(sTable))
		{
			return 0;
		}
		else if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
		{
			return -1; // program not started as a service
		}
		else
		{
			return -2;
		}
	}
}

int RunFW(int port)
{
	// Setup logging the way we want (this could be configured)
	auto lw = Logging::LogWorker::GetWorker();
	lw.AddAppender(new Logging::ConsoleAppender("FW"));

	auto lt = Threading::Thread(lw);
	lt.Start();

	auto log = Logging::Logger::GetLogger("FW");

	// Create our main class
	auto fw = ForgottenWar(9005, log);
	fw.Initialize();

	FW::GAME_STATES result = FW::GAME_STATES::FWGAME_INVALID;

	while ((result = fw.GameLoop()) == FW::GAME_STATES::FWGAME_RUNNING) { }

	switch (result)
	{
	case FW::GAME_STATES::FWGAME_INVALID:
		return -1;
	case FW::GAME_STATES::FWGAME_STOPPING:
	default:
		return 0;
	}
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	return;
}
