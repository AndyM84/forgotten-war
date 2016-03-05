#pragma once

#include "../BaseClasses/NodeBase.hpp"

#include <memory>
#include <vector>

namespace N2f
{
	template<class T>
	class ChainHelper
	{
	public:
		typedef std::vector<std::shared_ptr<NodeBase<T>>> NodeList;

	protected:
		bool _doDebug = false, _isEvent = false;
		NodeList _nodes;

	public:

		/// <summary>
		/// Default constructor, chain will not be an event and will not debug.
		/// </summary>
		ChainHelper() : ChainHelper<T>(false, false) { }

		/// <summary>
		/// Main constructor, allows specification of event and debug statuses.
		/// </summary>
		/// <param name="IsEvent">
		/// true if ChainHelper instance is an event.
		/// </param>
		/// <param name="DoDebug">
		/// true if ChainHelper instance should show debug information.
		/// </param>
		ChainHelper(bool IsEvent, bool DoDebug)
		{
			this->_doDebug = DoDebug;
			this->_isEvent = IsEvent;

			return;
		}

		/// <summary>
		/// Virtual destructor for cleanup.
		/// </summary>
		virtual ~ChainHelper()
		{
			if (this->_nodes.size() > 0)
			{
				this->_nodes.clear();
			}

			return;
		}

		/// <summary>
		/// Returns the current list of nodes.
		/// </summary>
		/// <returns>
		/// Vector of NodeBase nodes linked into chain.
		/// </returns>
		const NodeList GetNodes()
		{
			return this->_nodes;
		}

		/// <summary>
		/// Whether or not this ChainHelper instance is producing debug information.
		/// </summary>
		/// <returns>
		/// true if producing debug information, false if not.
		/// </returns>
		bool IsDebug()
		{
			return this->_doDebug;
		}

		/// <summary>
		///	Whether or not this ChainHelper instance is an event, meaning it will only allow one NodeBase
		///	to be linked at any time.
		///	</summary>
		/// <returns>
		/// true if event, false if not.
		/// </returns>
		bool IsEvent()
		{
			return this->_isEvent;
		}

		/// <summary>
		/// Links a node into the ChainHelper instance.
		/// </summary>
		/// <param name="Node">
		/// The NodeBase node to link.
		/// </param>
		/// <returns>
		/// A reference to the ChainHelper instance.
		/// </returns>
		ChainHelper &LinkNode(std::shared_ptr<NodeBase<T>> Node)
		{
			if (!Node->IsValid())
			{
				return *this;
			}

			if (this->_isEvent && this->_nodes.size() == 1)
			{
				this->_nodes.pop_back();
			}

			this->_nodes.push_back(Node);

			return *this;
		}

		/// <summary>
		/// Triggers the traversal of the chain.
		/// </summary>
		/// <param name="Sender">
		/// [in,out] If non-null, the sender.
		/// </param>
		/// <param name="Dispatch">
		/// The dispatch to send along the chain.
		/// </param>
		/// <returns>
		/// true if traversal happens, false if it fails.
		/// </returns>
		bool Traverse(void *Sender, std::shared_ptr<T> Dispatch)
		{
			if (!this->instanceof<DispatchBase>(Dispatch))
			{
				return false;
			}

			if (this->_nodes.size() < 1)
			{
				return false;
			}
			else if (!Dispatch->IsValid())
			{
				return false;
			}
			else if (Dispatch->IsConsumable() && Dispatch->IsConsumed())
			{
				return false;
			}
			else
			{
				bool isConsumable = Dispatch->IsConsumable();

				if (this->_isEvent)
				{
					this->_nodes.back()->Process(Sender, Dispatch);
				}
				else
				{
					for (auto n : this->_nodes)
					{
						n->Process(Sender, Dispatch);

						if (isConsumable && Dispatch->IsConsumed())
						{
							break;
						}
					}
				}
			}

			return true;
		}

	private:
		template<typename Base, typename T>
		inline bool instanceof(const std::shared_ptr<T> ptr) {
			return std::is_base_of<Base, T>::value;
		}
	};
}
