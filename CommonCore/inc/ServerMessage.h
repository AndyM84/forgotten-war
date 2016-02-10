#pragma once

#include <Common/Types.h>
#include <N2f/N2f.h>

class ServerMessage : public N2f::DispatchBase
{
	virtual fwvoid Initialize();
	virtual fwvoid Initialize(const fwstr Message);
	virtual fwint NumResults();
	virtual fwvoid SetResult();
};
