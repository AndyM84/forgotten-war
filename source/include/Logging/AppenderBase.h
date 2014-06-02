#pragma once

#include <Common/Types.h>
#include "LogData.h"

namespace Chimera
{
	namespace Logging
	{
		class AppenderBase
		{
		public:
			virtual cxvoid DoAppend(LogData data) = 0;
			virtual cxbool ReOpen();
			virtual cxvoid Close();

		protected:
			AppenderBase(const cxstring name);

			cxstring m_Name;
		};
	};
};
