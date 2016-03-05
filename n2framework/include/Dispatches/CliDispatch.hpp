#pragma once

#include "../BaseClasses/DispatchBase.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace N2f
{
	class CliDispatch : public N2f::DispatchBase
	{
	public:
		CliDispatch();
		~CliDispatch();

		/* N2f::DispatchBase methods */

		virtual void Initialize();
		virtual int NumResults();
		virtual void SetResult();

		/* Now our versions */

		CliDispatch &Initialize(int argc, char *argv[]);
		const bool IsWindows();
		const std::string GetParameterString();
		const std::vector<std::string> GetRawParameters();
		const std::map<std::string, std::string> GetParameterMap(bool invariantKey);

	protected:
		typedef std::pair<std::string, std::string> MAP_PAIR;

		std::map<std::string, std::string> mappedParams, mappedInvariantParams;
		std::vector<std::string> rawParams;
		std::string raw;

		void insertMappedPair(std::string key, std::string val);
	};
}
