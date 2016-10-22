#include <ForgottenWar.h>

#include <iostream>
#include <fstream>

#if defined(FW_WINDOWS)
	#define IS_DEBUG IsDebuggerPresent() ? true : false
#else
	#define IS_DEBUG false
#endif

HANDLE g_StopEvent;
DWORD g_CurrentState = 0;
bool g_SystemShutdown = false;
Threading::Thread *lt;
ForgottenWar *fw;

bool StartFW(int port);
FW::GAME_STATES RunFW();
int StopFW();

auto fwLog = Logging::Logger::GetLogger("FW");
auto fwLw = Logging::LogWorker::GetWorker();

int main(int argc, char *argv[])
{
	auto disp = CliDispatch();
	disp.Initialize(argc, argv);

	auto mappedArgs = disp.GetParameterMap(true);
	auto runIter = mappedArgs.find("run");

	if (runIter != mappedArgs.end() || IS_DEBUG)
	{
		int result = 0;

		if (disp.NumResults() == 1)
		{
			StartFW(9005);
		}
		else
		{
			if (disp.NumResults() == 2)
			{
				auto args = disp.GetRawParameters();

				StartFW(atoi(args[1].c_str()));
			}

			// TODO: Add some more stuff here, like perhaps look into log levels and what not
		}

		g_StopEvent = CreateEvent(NULL, true, false, NULL);

		while (RunFW() == FW::GAME_STATES::FWGAME_RUNNING)
		{
			if (g_StopEvent != NULL && WaitForSingleObject(g_StopEvent, 1) == WAIT_OBJECT_0)
			{
				result = StopFW();

				break;
			}
		}

		StopFW();

		return result;
	}
}

bool StartFW(int port)
{
	// TODO: This should work with some error reporting, probably

	lt = new Threading::Thread(fwLw);
	lt->Start();

	TCHAR szExePath[4096];
	GetModuleFileName(NULL, szExePath, 4096);
	fwstr exePath(szExePath);

	auto lpos = exePath.find_last_of('\\');

	if (lpos == fwstr::npos)
	{
		lpos = 0;
	}

	fwstr ep = exePath.substr(0, lpos) + "\\";

	// Setup logging the way we want (this could be configured)
	fwLw.AddAppender(new Logging::ConsoleAppender("FW"));
	fwLw.AddAppender(new Logging::FileAppender("FW", ep + "fw.log"));

	// Create our main class
	fw = new ForgottenWar(ep, 9005, fwLog);
	fw->Initialize();

	return true;
}

FW::GAME_STATES RunFW()
{
	return fw->GameLoop();
}

int StopFW()
{
	fw->Stop();
	lt->Terminate();
	lt->CloseThread();

	delete fw;
	delete lt;

	return 0;
}
