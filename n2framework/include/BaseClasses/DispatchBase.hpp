#pragma once

namespace N2f
{
	/// <summary>
	///	Abstract base class for dispatches used to carry information
	///	along chains.
	///	</summary>
	class DispatchBase
	{
	protected:
		bool _isConsumable = false, _isConsumed = false, _isStateful = false, _isValid = false;

		/// <summary>
		/// Marks the dispatch as consumable.
		/// </summary>
		void MakeConsumable();

		/// <summary>
		/// Marks the dispatch as stateful, meaning it will accept multiple results.
		/// </summary>
		void MakeStateful();

		/// <summary>
		/// Marks the dispatch as valid for use in a chain.
		/// </summary>
		void MakeValid();

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
		virtual void Initialize() = 0;

		/// <summary>
		/// Method to return the number of results given to the dispatch.
		/// </summary>
		/// <returns>
		/// The total number of results.
		/// </returns>
		virtual int NumResults() = 0;

		/// <summary>
		///	Base method to set results in the dispatch.  Will almost always be
		///	overloaded, but may be used if the result is able to be determined
		///	by the dispatch without external information (calculations or system
		///	based).
		/// </summary>
		virtual void SetResult() = 0;

		/// <summary>
		/// Consumes the dispatch, ending any non-event chain traversals.
		/// </summary>
		/// <returns>
		/// true if dispatch is consumable and is not already consumed, false otherwise.
		/// </returns>
		bool Consume();

		/// <summary>
		/// Whether or not the dispatch can be consumed.
		/// </summary>
		/// <returns>
		/// true if consumable, false if not.
		/// </returns>
		bool IsConsumable();

		/// <summary>
		/// Whether or not the dispatch has been consumed.
		/// </summary>
		/// <returns>
		/// true if consumed, false if not.
		/// </returns>
		bool IsConsumed();

		/// <summary>
		/// Whether or not the dispatch is stateful (can contain multiple results).
		/// </summary>
		/// <returns>
		/// true if stateful, false if not.
		/// </returns>
		bool IsStateful();

		/// <summary>
		/// Whether or not the dispatch is valid for use in a chain.
		/// </summary>
		/// <returns>
		/// true if valid, false if not.
		/// </returns>
		bool IsValid();
	};
}
