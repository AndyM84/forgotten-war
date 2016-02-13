#pragma once

// Core includes
#include <Common/Types.h>
#include <Logging/Logger.h>
#include <Threading/LockCriticalSection.h>
#include <Threading/Threadable.h>

class Game : public Threading::Threadable
{
public:
	virtual fwvoid Run();
};
