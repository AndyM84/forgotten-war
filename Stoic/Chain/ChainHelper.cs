using System.Collections.Generic;

namespace Stoic.Chain
{
	public class ChainHelper<DispatchType, ResultType, CollectionType>
		where DispatchType : DispatchBase<ResultType, CollectionType>
		where CollectionType : ICollection<ResultType>, new()
	{
		protected bool _Debug;
		protected bool _Event;
		protected List<string> _LogMessages;
		protected List<NodeBase<DispatchType, ResultType, CollectionType>> _Nodes;

		public bool Debug { get { return this._Debug; } }
		public bool Event { get { return this._Event; } }
		public List<string> LogMessages { get { return new List<string>(this._LogMessages); } }
		public List<NodeBase<DispatchType, ResultType, CollectionType>> Nodes { get { return new List<NodeBase<DispatchType, ResultType, CollectionType>>(this._Nodes); } }


		public ChainHelper(bool IsEvent = false, bool IsDebug = false)
		{
			this._Debug = IsDebug;
			this._Event = IsEvent;
			this._LogMessages = new List<string>();
			this._Nodes = new List<NodeBase<DispatchType, ResultType, CollectionType>>();

			return;
		}


		public ChainHelper<DispatchType, ResultType, CollectionType> LinkNode(NodeBase<DispatchType, ResultType, CollectionType> Node)
		{
			if (!Node.IsValid()) {
				this._LogMessages.Add("Invalid node, could not link into chain");

				return this;
			}

			if (this._Event) {
				this._Nodes = new List<NodeBase<DispatchType, ResultType, CollectionType>>();
				this._LogMessages.Add("Reset chain node stack due to event setting");
			}

			this._Nodes.Add(Node);
			this._LogMessages.Add("Linked " + Node.Key + " (v" + Node.Version + ") node into chain");

			return this;
		}

		
	}
}
