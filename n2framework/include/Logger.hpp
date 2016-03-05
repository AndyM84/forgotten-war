#pragma once

#include "BaseClasses/NodeBase.hpp"
#include "Config.hpp"
#include "Helpers/ChainHelper.hpp"
#include "Logger.hpp"

#include <map>
#include <vector>

namespace N2f
{
	class Logger
	{
	protected:
		//ChainHelper _logChain, _outputChain;
		LoggerConfig _config;
	};
}
