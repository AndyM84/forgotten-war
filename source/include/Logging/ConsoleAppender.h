#pragma once

#include <Logging\AppenderBase.h>

#include <iostream>

namespace Chimera
{
	namespace Logging
	{
		class ConsoleAppender : public AppenderBase
		{
		public:
			ConsoleAppender(const cxstring name);
			virtual cxvoid DoAppend(LogData data);
		};
	};
};
