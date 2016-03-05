#include "Dispatches/CliDispatch.hpp"

namespace N2f
{
	CliDispatch::CliDispatch()
	{
		this->MakeConsumable();

		return;
	}

	CliDispatch::~CliDispatch()
	{
		this->rawParams.clear();
		this->mappedParams.clear();
		this->mappedInvariantParams.clear();

		return;
	}

	/* N2f::DispatchBase methods */

	void CliDispatch::Initialize()
	{
		return;
	}

	int CliDispatch::NumResults()
	{
		return (int)this->rawParams.size();
	}

	void CliDispatch::SetResult()
	{
		return;
	}

	/* Now our versions */

	CliDispatch &CliDispatch::Initialize(int argc, char *argv[])
	{
		if (argc < 1)
		{
			return *this;
		}

		for (int i = 0; i < argc; i++)
		{
			std::string tmp = argv[i];
			this->rawParams.push_back(tmp);

			if (this->raw.length() > 0)
			{
				this->raw += " ";
			}

			this->raw += tmp;

			if (tmp.substr(0, 1) == "-" && tmp.length() > 1)
			{
				auto param = tmp.substr((tmp.substr(1, 1) == "-") ? 2 : 1);
				auto eq = param.find('=');
				auto ds = param.find('-');

				if (eq != std::string::npos && eq != (param.length() - 1))
				{
					this->insertMappedPair(param.substr(0, eq), param.substr(eq + 1));
				}
				else if (ds != std::string::npos && ds != (param.length() - 1))
				{
					this->insertMappedPair(param.substr(0, ds), param.substr(ds + 1));
				}
				else if ((i + 1) < argc)
				{
					this->insertMappedPair(param, argv[++i]);
				}
				else
				{
					this->insertMappedPair(param, "true");
				}
			}
			else
			{
				auto eq = tmp.find('=');

				if (eq != std::string::npos)
				{
					this->insertMappedPair(tmp.substr(0, eq), tmp.substr(eq + 1));
				}
				else
				{
					this->insertMappedPair(tmp, "true");
				}
			}
		}

		this->MakeValid();

		return *this;
	}

	const bool CliDispatch::IsWindows()
	{
	#if defined(_WIN32)
		return true;
	#else
		return false;
	#endif
	}

	const std::string CliDispatch::GetParameterString()
	{
		return this->raw;
	}

	const std::vector<std::string> CliDispatch::GetRawParameters()
	{
		return this->rawParams;
	}

	const std::map<std::string, std::string> CliDispatch::GetParameterMap(bool invariantKey)
	{
		if (invariantKey)
		{
			return this->mappedInvariantParams;
		}

		return this->mappedParams;
	}

	void CliDispatch::insertMappedPair(std::string key, std::string val)
	{
		this->mappedParams.insert(MAP_PAIR(key, val));

		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		this->mappedInvariantParams.insert(MAP_PAIR(key, val));

		return;
	}
}
