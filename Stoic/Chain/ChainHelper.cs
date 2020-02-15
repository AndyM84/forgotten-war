using System.Collections.Generic;

namespace Stoic.Chain
{
	public class ChainHelper<DispatchType, ResultType, CollectionType>
		where DispatchType : DispatchBase<ResultType, CollectionType>
		where CollectionType : ICollection<ResultType>, new()
	{
		public delegate void LogHandler(string Message);

		protected bool _Debug;
		protected bool _Event;
		protected List<NodeBase<DispatchType, ResultType, CollectionType>> _Nodes;

		public bool IsDebug { get { return this._Debug; } }
		public bool IsEvent { get { return this._Event; } }
		public LogHandler Logger { get; set; }
		public List<NodeBase<DispatchType, ResultType, CollectionType>> Nodes { get { return new List<NodeBase<DispatchType, ResultType, CollectionType>>(this._Nodes); } }


		public ChainHelper(bool IsEvent = false, bool IsDebug = false)
		{
			this._Debug = IsDebug;
			this._Event = IsEvent;
			this._Nodes = new List<NodeBase<DispatchType, ResultType, CollectionType>>();

			return;
		}


		public ChainHelper<DispatchType, ResultType, CollectionType> LinkNode(NodeBase<DispatchType, ResultType, CollectionType> Node)
		{
			if (!Node.IsValid()) {
				this.Log("Invalid node, could not link into chain");

				return this;
			}

			if (this._Event) {
				this._Nodes = new List<NodeBase<DispatchType, ResultType, CollectionType>>();
				this.Log("Reset chain node stack due to event setting");
			}

			this._Nodes.Add(Node);
			this.Log("Linked " + Node.Key + " (v" + Node.Version + ") node into chain");

			return this;
		}

		public bool Traverse(ref DispatchType Dispatch, object Sender = null)
		{
			if (this._Nodes.Count < 1) {
				this.Log("Attempted to traverse chain with no nodes");

				return false;
			}

			if (!Dispatch.IsValid) {
				this.Log("Attempted to traverse chain with invalid dispatch: " + Dispatch);

				return false;
			}

			if (Dispatch.IsConsumable && Dispatch.IsConsumed) {
				this.Log("Attempted to traverse chain with consumed dispatch: " + Dispatch);

				return false;
			}

			if (Sender == null) {
				Sender = this;
			}

			var isConsumable = Dispatch.IsConsumable;

			if (this._Event) {
				this.Log("Sending dispatch (" + Dispatch + ") to event node: " + this._Nodes[0]);

				this._Nodes[0].Process(ref Sender, ref Dispatch);
			} else {
				foreach (var n in this._Nodes) {
					this.Log("Sending dispatch (" + Dispatch + ") to event node: " + n);

					n.Process(ref Sender, ref Dispatch);

					if (isConsumable && Dispatch.IsConsumed) {
						this.Log("Dispatch (" + Dispatch + ") consumed by node: " + n);

						break;
					}
				}
			}

			return true;
		}

		public void Log(string Message)
		{
			if (this._Debug) {
				this.Logger(Message);
			}

			return;
		}
	}
}
