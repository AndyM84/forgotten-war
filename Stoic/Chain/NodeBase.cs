using System.Collections.Generic;

namespace Stoic.Chain
{
	public abstract class NodeBase<DispatchType, ResultType, CollectionType>
		where DispatchType : DispatchBase<ResultType, CollectionType>
		where CollectionType : ICollection<ResultType>, new()
	{
		protected string _Key;
		protected string _Version;

		public string Key { get { return this._Key; } }
		public string Version { get { return this._Version; } }


		protected NodeBase(string Key, string Version)
		{
			this._Key = Key;
			this._Version = Version;

			return;
		}


		public bool IsValid()
		{
			return !string.IsNullOrWhiteSpace(this._Key) && !string.IsNullOrWhiteSpace(this._Version);
		}

		public abstract void Process(object Sender, DispatchType Dispatch);
	}
}
