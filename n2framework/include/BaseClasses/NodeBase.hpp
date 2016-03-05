#pragma once

#include "DispatchBase.hpp"
#include <memory>

#define MAX_NODE_KEY_LENGTH 256
#define MAX_NODE_VER_LENGTH 12

namespace N2f
{
	/// <summary>
	///	Abstract base class for nodes used to process information
	///	sent along chains.
	/// </summary>
	template<class T>
	class NodeBase
	{
	protected:
		char _key[MAX_NODE_KEY_LENGTH], _version[MAX_NODE_VER_LENGTH];

	private:
		/// <summary>
		/// Sets the node's key value provided it hasn't already been set.
		/// </summary>
		/// <param name="Key">
		/// The value to use for the node's key, must be not null and have characters.
		/// </param>
		void SetKey(const char *Key)
		{
			if (!Key || strlen(Key) > MAX_NODE_KEY_LENGTH || strlen(Key) < 1)
			{
				return;
			}

			// TODO: Add logging here in the future, for now we're just looking at basic usage
			if (!this->_key || strlen(this->_key) < 1)
			{
#if defined(_MSC_VER)
				strcpy_s(this->_key, sizeof(this->_key), Key);
#else
				strcpy(this->_key, Key);
#endif
			}

			return;
		}

		/// <summary>
		/// Sets the node's version value provided it hasn't already been set.
		/// </summary>
		/// <param name="Version">
		/// The value to use for the node's version, must be not null and have characters.
		/// </param>
		void SetVersion(const char *Version)
		{
			if (!Version || strlen(Version) > MAX_NODE_VER_LENGTH || strlen(Version) < 1)
			{
				return;
			}

			// TODO: Add logging here in the future, for now we're just looking at basic usage
			if (!this->_version || strlen(this->_version) < 1)
			{
#if defined(_MSC_VER)
				strcpy_s(this->_version, sizeof(this->_version), Version);
#else
				strcpy(this->_version, Version);
#endif
			}

			return;
		}

	public:
		/// <summary>
		/// Initializes the node with its most basic information.
		/// </summary>
		/// <param name="Key">
		/// The value to use for the node's key. Must not be null and have characters.
		/// </param>
		/// <param name="Version">
		NodeBase(const char *Key, const char *Version)
		{
			if (!std::is_base_of<DispatchBase, T>::value)
			{
				return;
			}

			this->_key[0] = this->_version[0] = 0;

			this->SetKey(Key);
			this->SetVersion(Version);

			return;
		}

		/// <summary>
		/// Virtual destructor for cleanup.
		/// </summary>
		virtual ~NodeBase() { }

		/// <summary>
		/// Virtual method to receive a dispatch from a chain and process it.
		/// </summary>
		/// <param name="Sender">
		/// [in,out] If non-null, the sender.
		/// </param>
		/// <param name="Dispatch">
		/// The dispatch to process.
		/// </param>
		virtual void Process(void *Sender, std::shared_ptr<T> Dispatch) = 0;

		/// <summary>
		/// Returns the value of the node's key.
		/// </summary>
		/// <returns>
		/// null if it fails, else the key.
		/// </returns>
		const char *GetKey()
		{
			return this->_key;
		}

		/// <summary>
		/// Returns the value of the node's version.
		/// </summary>
		/// <returns>
		/// null if it fails, else the version.
		/// </returns>
		const char *GetVersion()
		{
			return this->_version;
		}

		/// <summary>
		///	Whether or not the node is valid for processing.  Valid nodes have both their key and
		///	version values set.
		///	</summary>
		/// <returns>
		/// true if valid, false if not.
		/// </returns>
		bool IsValid()
		{
			return strlen(this->_key) > 0 && strlen(this->_version) > 0;
		}
	};
}
