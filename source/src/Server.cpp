#include <ForgottenWar.h>

// https://msdn.microsoft.com/en-us/library/bb540475%28v=vs.85%29.aspx

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <WtsApi32.h>
#include <iostream>
#include <fstream>
#include <direct.h>

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Wtsapi32.lib")

#define SVCNAME TEXT("ForgottenWar")

#if defined(FW_WINDOWS)
	#define IS_DEBUG IsDebuggerPresent() ? true : false
#else
	#define IS_DEBUG false
#endif

SERVICE_STATUS_HANDLE g_ServiceStatusHandle;
HANDLE g_StopEvent;
DWORD g_CurrentState = 0;
bool g_SystemShutdown = false;
Threading::Thread *lt;
ForgottenWar *fw;

bool StartFW(int port);
FW::GAME_STATES RunFW();
int StopFW();

// SVC stuff
SERVICE_STATUS gSvcStatus;
SERVICE_STATUS_HANDLE gSvcStatusHandle;
HANDLE gSvcStopEvent = NULL;

void WINAPI SvcCtrlHandler(DWORD);
void WINAPI SvcMain(DWORD, LPTSTR*);
void ReportSvcStatus(DWORD, DWORD, DWORD);
void SvcInit(DWORD, LPTSTR*);
void SvcReportEvent(LPSTR);

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
	else
	{
		SERVICE_TABLE_ENTRY sTable[] = {
			{ SVCNAME, &SvcMain },
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

bool StartFW(int port)
{
	// TODO: This should work with some error reporting, probably

	// Setup logging the way we want (this could be configured)
	fwLw.AddAppender(new Logging::ConsoleAppender("FW"));

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

	// Create our main class
	fw = new ForgottenWar(exePath.substr(0, lpos) + "\\", 9005, fwLog);
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

void WINAPI SvcCtrlHandler(DWORD dwCtrl)
{
	switch (dwCtrl)
	{
	case SERVICE_CONTROL_STOP:
		ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
		SetEvent(gSvcStopEvent);
		ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);

		return;

	case SERVICE_CONTROL_INTERROGATE:
		break;

	default:
		break;
	}

	return;
}

void WINAPI SvcMain(DWORD dwArg, LPTSTR *lpszArgv)
{
	gSvcStatusHandle = RegisterServiceCtrlHandler(SVCNAME, SvcCtrlHandler);

	if (!gSvcStatusHandle)
	{
		SvcReportEvent(TEXT("RegisterServiceCtrlHandler"));

		return;
	}

	gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	gSvcStatus.dwServiceSpecificExitCode = 0;

	ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

	gSvcStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (gSvcStopEvent == NULL)
	{
		ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);

		return;
	}

	// TODO: Configuration of this somehow
	StartFW(9005);

	ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);

	while (true)
	{
		RunFW();

		if (WaitForSingleObject(gSvcStopEvent, 1) == WAIT_OBJECT_0)
		{
			ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

			break;
		}
	}

	StopFW();
	CloseHandle(gSvcStopEvent);

	ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);

	return;
}

void ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;

	gSvcStatus.dwCurrentState = dwCurrentState;
	gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
	gSvcStatus.dwWaitHint = dwWaitHint;

	if (dwCurrentState == SERVICE_START_PENDING)
	{
		gSvcStatus.dwControlsAccepted = 0;
	}
	else
	{
		gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	}

	if (dwCurrentState == SERVICE_RUNNING || dwCurrentState == SERVICE_STOPPED)
	{
		gSvcStatus.dwCheckPoint = 0;
	}
	else
	{
		gSvcStatus.dwCheckPoint = dwCheckPoint++;
	}

	SetServiceStatus(gSvcStatusHandle, &gSvcStatus);

	return;
}

void SvcReportEvent(LPSTR szFunction)
{
	HANDLE hEventSource;
	LPCTSTR lpszStrings[2];
	TCHAR Buffer[80];

	hEventSource = RegisterEventSource(NULL, SVCNAME);

	if (NULL != hEventSource)
	{
		StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

		lpszStrings[0] = SVCNAME;
		lpszStrings[1] = Buffer;

		ReportEvent(hEventSource,        // event log handle
		EVENTLOG_ERROR_TYPE, // event type
		0,                   // event category
		(DWORD)0xC0020001L,           // event identifier
		NULL,                // no security identifier
		2,                   // size of lpszStrings array
		0,                   // no binary data
		lpszStrings,         // array of strings
		NULL);               // no binary data

		DeregisterEventSource(hEventSource);
	}

	return;
}
