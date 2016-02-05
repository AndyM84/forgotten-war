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
	l.SetDefaultLevel(Logging::LogLevel::LOG_TRACE);

	auto fw = ForgottenWar(9005, l);
	fw.Start();

	std::cout << "Waiting for newline input to close..";
	std::cin.get();

	lt.Terminate();

	return 0;
}
