#include <Logging\ConsoleAppender.h>

namespace Chimera
{
	namespace Logging
	{
		ConsoleAppender::ConsoleAppender(const cxstring name)
			: AppenderBase(name)
		{ }

		cxvoid ConsoleAppender::DoAppend(LogData data)
		{
			std::cout << "[" << this->m_Name << " @ datetime-here] (" << data.GetLevel() << ") " << data.GetKey() << ": " << data.GetMsg() << std::endl;

			return;
		}
	};
};
