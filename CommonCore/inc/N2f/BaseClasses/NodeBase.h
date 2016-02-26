#pragma once

#include <Common/Types.h>
#include <N2f/BaseClasses/DispatchBase.h>
#include <memory>
#include <string.h>

#define MAX_NODE_KEY_LENGTH 256
#define MAX_NODE_VER_LENGTH 12

namespace N2f
{
	/// <summary>
	///	Abstract base class for nodes used to process information
	///	sent along chains.
	/// </summary>
	class NodeBase
	{
	protected:
		fwchar _key[MAX_NODE_KEY_LENGTH], _version[MAX_NODE_VER_LENGTH];

		/// <summary>
		/// Sets the node's key value provided it hasn't already been set.
		/// </summary>
		/// <param name="Key">
		/// The value to use for the node's key, must be not null and have characters.
		/// </param>
		fwvoid SetKey(const fwchar *Key);

		/// <summary>
		/// Sets the node's version value provided it hasn't already been set.
		/// </summary>
		/// <param name="Version">
		/// The value to use for the node's version, must be not null and have characters.
		/// </param>
		fwvoid SetVersion(const fwchar *Version);

	public:

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
		virtual fwvoid Process(fwvoid *Sender, std::shared_ptr<DispatchBase> Dispatch) = 0;

		/// <summary>
		/// Returns the value of the node's key.
		/// </summary>
		/// <returns>
		/// null if it fails, else the key.
		/// </returns>
		const fwchar *GetKey();

		/// <summary>
		/// Returns the value of the node's version.
		/// </summary>
		/// <returns>
		/// null if it fails, else the version.
		/// </returns>
		const fwchar *GetVersion();

		/// <summary>
		///	Whether or not the node is valid for processing.  Valid nodes have both their key and
		///	version values set.
		///	</summary>
		/// <returns>
		/// true if valid, false if not.
		/// </returns>
		fwbool IsValid();
	};
}
