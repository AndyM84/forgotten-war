#include <ForgottenWar.h>

int main()
{
	// Setup logging
	auto logworker = Logging::LogWorker::GetWorker();
	logworker.AddAppender(new Logging::ConsoleAppender("FW"));

	auto lt = Threading::Thread(logworker);
	lt.Start();

	auto l = Logging::Logger::GetLogger("FW");
	//l.SetReportingLevel(Logging::LogLevel::LOG_ERROR | Logging::LogLevel::LOG_CRITICAL | Logging::LogLevel::LOG_WARN);

	auto fw = ForgottenWar(9005, l);
	fw.Start();

	std::cout << "Waiting for newline input to close..";
	std::cin.get();

	lt.Terminate();

	return 0;
}
