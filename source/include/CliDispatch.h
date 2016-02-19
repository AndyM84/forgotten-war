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
	const std::vector<const fwstr> GetRawParameters();
	const std::map<const fwstr, const fwstr> GetParameterMap();

protected:
	std::map<const fwstr, const fwstr> mappedParams, mappedInvariantParams;
	std::vector<const fwstr> rawParams;
	fwstr raw;

	fwvoid Process();
};
