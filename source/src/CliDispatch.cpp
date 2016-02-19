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

		if (tmp.substr(0, 1) == "-" && tmp.length() > 1)
		{
			auto param = tmp.substr((tmp.substr(1, 1) == "-") ? 2 : 1);
			auto eq = param.find('=');

			if (eq != std::string::npos)
			{

			}
		}
		else
		{
			auto eq = tmp.find('=');

			if (eq != std::string::npos)
			{
				auto key = tmp.substr(0, eq);
				auto val = tmp.substr(eq + 1);

				this->mappedParams.insert(std::pair<const fwstr, const fwstr>(key, val));

				std::transform(key.begin(), key.end(), key.begin(), ::tolower);
				this->mappedInvariantParams.insert(std::pair<const fwstr, const fwstr>(key, val));
			}
			else
			{
				this->mappedParams.insert(std::pair<const fwstr, const fwstr>(tmp, "true"));

				std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
				this->mappedInvariantParams.insert(std::pair<const fwstr, const fwstr>(tmp, "true"));
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

const std::vector<const fwstr> CliDispatch::GetRawParameters()
{

}

const std::map<const fwstr, const fwstr> CliDispatch::GetParameterMap()
{

}

fwvoid CliDispatch::Process()
{
	return;
}
