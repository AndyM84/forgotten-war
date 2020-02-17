using System.Collections.Generic;

using Stoic.Chain;

namespace FW.Core
{
	public class TickDispatch : DispatchBase<Command, List<Command>>
	{
		protected List<Command> _Commands;

		public List<Command> Commands { get { return new List<Command>(this._Commands); } }


		public override void Initialize()
		{
			throw new System.NotImplementedException();
		}

		public void Initialize(List<Command> Commands)
		{
			this._Commands = new List<Command>(Commands);
			this.MakeStateful();

			if (this._Commands.Count > 0) {
				this.MakeValid();
			}

			return;
		}

		public void SendToUser(int ID, string Message)
		{
			this._Commands.Add(new Command {
				Contents = Message,
				ID = ID,
				Type = CommandTypes.SEND
			});

			return;
		}
	}
}
