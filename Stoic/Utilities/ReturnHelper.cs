using System.Collections.Generic;

namespace Stoic.Utilities
{
	public enum ReturnHelperStatuses
	{
		BAD,
		GOOD
	}

	public class ReturnHelper<ResultType>
	{
		protected List<string> _Messages;
		protected List<ResultType> _Results;
		protected ReturnHelperStatuses _Status;

		public bool IsBad { get { return this._Status == ReturnHelperStatuses.BAD; } }
		public bool IsGood { get { return this._Status == ReturnHelperStatuses.GOOD; } }
		public List<string> Messages { get { return this._Messages; } }
		public List<ResultType> Results { get { return this._Results; } }
		public ReturnHelperStatuses Status { get { return this._Status; } }


		public ReturnHelper()
		{
			this._Messages = new List<string>();
			this._Results = new List<ResultType>();

			return;
		}


		public ReturnHelper(ReturnHelperStatuses Status)
		{
			this._Messages = new List<string>();
			this._Results = new List<ResultType>();
			this._Status = Status;

			return;
		}

		public void AddMessage(string Message)
		{
			this._Messages.Add(Message);

			return;
		}

		public void AddMessage(string[] Messages)
		{
			foreach (var m in Messages) {
				this._Messages.Add(m);
			}

			return;
		}

		public void AddMessage(ICollection<string> Messages)
		{
			foreach (var m in Messages) {
				this._Messages.Add(m);
			}

			return;
		}

		public void AddResult(ResultType Result)
		{
			this._Results.Add(Result);

			return;
		}

		public void AddResult(ResultType[] Results)
		{
			foreach (var r in Results) {
				this._Results.Add(r);
			}

			return;
		}

		public void AddResult(ICollection<ResultType> Results)
		{
			foreach (var r in Results) {
				this._Results.Add(r);
			}

			return;
		}

		public void MakeBad()
		{
			this._Status = ReturnHelperStatuses.BAD;

			return;
		}

		public void MakeGood()
		{
			this._Status = ReturnHelperStatuses.GOOD;

			return;
		}
	}
}
