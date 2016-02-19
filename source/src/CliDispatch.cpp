#include <CliDispatch.h>

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

fwvoid CliDispatch::Initialize()
{
	return;
}

fwint CliDispatch::NumResults()
{
	return this->rawParams.size();
}

fwvoid CliDispatch::SetResult()
{
	return;
}

/* Now our versions */

CliDispatch &CliDispatch::Initialize(fwint argc, fwchar *argv[])
{
	if (argc < 1)
	{
		return *this;
	}

	for (fwint i = 0; i < argc; i++)
	{
		fwstr tmp = argv[i];
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

	return *this;
}

const fwbool CliDispatch::IsWindows()
{
#if defined(FW_WINDOWS)
	return true;
#else
	return false;
#endif
}

const fwstr CliDispatch::GetParameterString()
{
	return this->raw;
}

const std::vector<fwstr> CliDispatch::GetRawParameters()
{
	return this->rawParams;
}

const std::map<fwstr, fwstr> CliDispatch::GetParameterMap(fwbool invariantKey)
{
	if (invariantKey)
	{
		return this->mappedInvariantParams;
	}

	return this->mappedParams;
}

fwvoid CliDispatch::insertMappedPair(fwstr key, fwstr val)
{
	this->mappedParams.insert(MAP_PAIR(key, val));

	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	this->mappedInvariantParams.insert(MAP_PAIR(key, val));

	return;
}
