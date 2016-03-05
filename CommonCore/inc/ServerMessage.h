#pragma once

#include <Common/Types.h>
#include <N2f.hpp>

#include <algorithm>

class ServerMessage : public N2f::DispatchBase
{
public:
	ServerMessage();

	virtual fwvoid Initialize();
	virtual fwvoid Initialize(const fwstr Message);
	virtual const fwstr GetCmd() const;
	virtual const fwstr GetRaw() const;
	virtual const fwstr GetSansCmd() const;
	virtual const std::vector<fwstr> GetTokens() const;
	virtual fwint NumResults();
	virtual fwvoid SetResult();

protected:
	std::vector<fwstr> tokens;
	fwstr raw, cmd, sansCmd;
	bool hasLinefeed;
};
