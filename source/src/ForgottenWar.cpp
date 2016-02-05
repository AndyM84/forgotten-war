#include <ForgottenWar.h>

int main()
{
	// Setup logging
	auto consolelogger = Logging::ConsoleAppender("FW");
	auto logworker = Logging::LogWorker::GetWorker();
	logworker.AddAppender(&consolelogger);

	auto lt = Threading::Thread(logworker);
	lt.Start();

	auto l = Logging::Logger::GetLogger("FW");

	l.Info("This is an info message.");

	std::cin.get();

	lt.Terminate();

	return 0;
}
