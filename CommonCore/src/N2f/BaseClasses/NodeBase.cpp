#include <N2f/BaseClasses/NodeBase.h>

namespace N2f
{
	fwvoid NodeBase::SetKey(const fwchar *Key)
	{
		// TODO: Add logging here in the future, for now we're just looking at basic usage
		if ((this->_key == NULL || strlen(this->_key) < 1) && Key != NULL && strlen(Key) > 0)
		{
			strcpy(this->_key, Key);
		}

		return;
	}

	fwvoid NodeBase::SetVersion(const fwchar *Version)
	{
		// TODO: Add logging here in the future, for now we're just looking at basic usage
		if ((this->_version == NULL || strlen(this->_version) < 1) && Version != NULL && strlen(Version) > 0)
		{
			strcpy(this->_version, Version);
		}

		return;
	}

	const fwchar *NodeBase::GetKey()
	{
		return this->_key;
	}

	const fwchar *NodeBase::GetVersion()
	{
		return this->_version;
	}

	fwbool NodeBase::IsValid()
	{
		return this->_key != NULL && strlen(this->_key) > 0 && this->_version != NULL && strlen(this->_version) > 0;
	}
}
