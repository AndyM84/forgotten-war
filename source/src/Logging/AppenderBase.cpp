#include <Logging\AppenderBase.h>

namespace Chimera
{
	namespace Logging
	{
		cxbool AppenderBase::ReOpen() { return true; }
		cxvoid AppenderBase::Close() { }

		AppenderBase::AppenderBase(const cxstring name)
		{
			this->m_Name = name;

			return;
		}
	};
};
