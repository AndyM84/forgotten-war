#pragma once

#include <Common/Types.h>
#include <N2f/N2f.h>

class ServerMessage : public N2f::DispatchBase
{
public:
	ServerMessage();

	virtual fwvoid Initialize();
	virtual fwvoid Initialize(const fwstr Message);
	virtual const fwstr GetCmd();
	virtual const fwstr GetRaw();
	virtual const fwstr GetSansCmd();
	virtual const std::vector<fwstr> GetTokens();
	virtual fwint NumResults();
	virtual fwvoid SetResult();

protected:
	std::vector<fwstr> tokens;
	fwstr raw, cmd, sansCmd;
};
