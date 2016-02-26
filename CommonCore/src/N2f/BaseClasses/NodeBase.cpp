#include <N2f/BaseClasses/NodeBase.h>

namespace N2f
{
	fwvoid NodeBase::SetKey(const fwchar *Key)
	{
		if (!Key || strlen(Key) > MAX_NODE_KEY_LENGTH || strlen(Key) < 1)
		{
			return;
		}

		// TODO: Add logging here in the future, for now we're just looking at basic usage
		if (!this->_key || strlen(this->_key) < 1)
		{
#if defined(FW_WINDOWS)
			strcpy_s(this->_key, sizeof(this->_key), Key);
#elif defined(FW_UNIX)
			strcpy(this->_key, Key);
#endif
		}

		return;
	}

	fwvoid NodeBase::SetVersion(const fwchar *Version)
	{
		if (!Version || strlen(Version) > MAX_NODE_VER_LENGTH || strlen(Version) < 1)
		{
			return;
		}

		// TODO: Add logging here in the future, for now we're just looking at basic usage
		if (!this->_version || strlen(this->_version) < 1)
		{
#if defined(FW_WINDOWS)
			strcpy_s(this->_version, sizeof(this->_version), Version);
#elif defined(FW_UNIX)
			strcpy(this->_version, Version);
#endif
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
