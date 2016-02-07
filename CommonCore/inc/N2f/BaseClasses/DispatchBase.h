#pragma once

#include <Common/Types.h>

namespace N2f
{
	/// <summary>
	///	Abstract base class for dispatches used to carry information
	///	along chains.
	///	</summary>
	class DispatchBase
	{
	protected:
		fwbool _isConsumable = false, _isConsumed = false, _isStateful = false, _isValid = false;

		/// <summary>
		/// Marks the dispatch as consumable.
		/// </summary>
		fwvoid MakeConsumable();

		/// <summary>
		/// Marks the dispatch as stateful, meaning it will accept multiple results.
		/// </summary>
		fwvoid MakeStateful();

		/// <summary>
		/// Marks the dispatch as valid for use in a chain.
		/// </summary>
		fwvoid MakeValid();

	public:

		/// <summary>
		/// Virtual destructor for cleanup.
		/// </summary>
		virtual ~DispatchBase() { }

		/// <summary>
		///	Base initializer, only useful when dispatch is intended to
		///	collect information passively and not start with information
		///	supplied to it.
		/// </summary>
		virtual fwvoid Initialize() = 0;

		/// <summary>
		/// Method to return the number of results given to the dispatch.
		/// </summary>
		/// <returns>
		/// The total number of results.
		/// </returns>
		virtual fwint NumResults() = 0;

		/// <summary>
		///	Base method to set results in the dispatch.  Will almost always be
		///	overloaded, but may be used if the result is able to be determined
		///	by the dispatch without external information (calculations or system
		///	based).
		/// </summary>
		virtual fwvoid SetResult() = 0;

		/// <summary>
		/// Consumes the dispatch, ending any non-event chain traversals.
		/// </summary>
		/// <returns>
		/// true if dispatch is consumable and is not already consumed, false otherwise.
		/// </returns>
		fwbool Consume();

		/// <summary>
		/// Whether or not the dispatch can be consumed.
		/// </summary>
		/// <returns>
		/// true if consumable, false if not.
		/// </returns>
		fwbool IsConsumable();

		/// <summary>
		/// Whether or not the dispatch has been consumed.
		/// </summary>
		/// <returns>
		/// true if consumed, false if not.
		/// </returns>
		fwbool IsConsumed();

		/// <summary>
		/// Whether or not the dispatch is stateful (can contain multiple results).
		/// </summary>
		/// <returns>
		/// true if stateful, false if not.
		/// </returns>
		fwbool IsStateful();

		/// <summary>
		/// Whether or not the dispatch is valid for use in a chain.
		/// </summary>
		/// <returns>
		/// true if valid, false if not.
		/// </returns>
		fwbool IsValid();
	};
}
