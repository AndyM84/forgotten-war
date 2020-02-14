using System;
using System.Collections.Generic;

namespace Stoic.Chain
{
	public abstract class DispatchBase<ResultType, CollectionType>
		where CollectionType : ICollection<ResultType>, new()
	{
		protected DateTime? _CalledDateTime;
		protected bool _IsConsumable;
		protected bool _IsConsumed;
		protected bool _IsStateful;
		protected bool _IsValid;
		protected CollectionType _Results;

		public DateTime? CalledDateTime { get { return this._CalledDateTime; } }
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
			this._CalledDateTime = DateTime.Now;
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

		public override string ToString()
		{
			return string.Format(
				"{0} object: {{ \"calledDateTime\": \"{1}\", \"isConsumable\": \"{2}\", \"isStateful\": \"{3}\", \"isConsumed\": \"{4}\" }}",
				new System.Diagnostics.StackFrame(1).GetMethod().Name,
				(this._CalledDateTime.HasValue) ? this._CalledDateTime.Value.ToString("yyyy-MM-dd HH:mm:ss") : "N/A",
				(this._IsConsumable) ? "Yes" : "No",
				(this._IsStateful) ? "Yes" : "No",
				(this._IsConsumed) ? "Yes" : "No"
			);
		}
	}
}
