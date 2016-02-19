#pragma once

#include <CommonCore.h>

class CliDispatch : public N2f::DispatchBase
{
public:
	CliDispatch();
	~CliDispatch();

	/* N2f::DispatchBase methods */

	virtual fwvoid Initialize();
	virtual fwint NumResults();
	virtual fwvoid SetResult();

	/* Now our versions */

	CliDispatch &Initialize(fwint argc, fwchar *argv[]);
	const fwbool IsWindows();
	const fwstr GetParameterString();
	const std::vector<fwstr> GetRawParameters();
	const std::map<fwstr, fwstr> GetParameterMap(fwbool invariantKey);

protected:
	typedef std::pair<fwstr, fwstr> MAP_PAIR;

	std::map<fwstr, fwstr> mappedParams, mappedInvariantParams;
	std::vector<fwstr> rawParams;
	fwstr raw;

	fwvoid insertMappedPair(fwstr key, fwstr val);
};
