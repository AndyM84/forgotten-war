using System;
using System.Collections.Generic;

namespace Stoic.Chain
{
	public abstract class DispatchBase<ResultType, CollectionType>
		where CollectionType : ICollection<ResultType>, new()
	{
		protected DateTime _CalledDateTime;
		protected bool _IsConsumable;
		protected bool _IsConsumed;
		protected bool _IsStateful;
		protected bool _IsValid;
		protected CollectionType _Results;

		public DateTime CalledDateTime { get { return this._CalledDateTime; } }
		public bool IsConsumable { get { return this._IsConsumable; } }
		public bool IsConsumed { get { return this._IsConsumed; } }
		public bool IsStateful { get { return this._IsStateful; } }
		public bool IsValid { get { return this._IsValid; } }
		public CollectionType Results { get { return this._Results; } }


		public bool Consume()
		{
			if (this._IsConsumable && !this._IsConsumed) {
				this._IsConsumed = true;

				return true;
			}

			return false;
		}

		public abstract void Initialize();

		protected void MakeConsumable()
		{
			this._IsConsumable = true;

			return;
		}

		protected void MakeStateful()
		{
			this._IsStateful = true;

			return;
		}

		protected void MakeValid()
		{
			this._IsValid = true;

			return;
		}

		public DispatchBase<ResultType, CollectionType> SetResult(ResultType Result)
		{
			if (!this._IsStateful) {
				this._Results = new CollectionType();
			}

			this._Results.Add(Result);

			return this;
		}
	}
}
