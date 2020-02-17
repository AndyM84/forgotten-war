using System.Collections.Generic;

using Stoic.Chain;

namespace FW.Core
{
	public class TickDispatch : DispatchBase<Command, List<Command>>
	{
		protected List<Command> _Commands;
		protected Game.State _State;

		public List<Command> Commands { get { return new List<Command>(this._Commands); } }
		public ref Game.State State { get { return ref this._State; } }


		public override void Initialize()
		{
			throw new System.NotImplementedException();
		}

		public void Initialize(ref Game.State State, List<Command> Commands)
		{
			this._Commands = new List<Command>(Commands);
			this._State = State;

			this.MakeStateful();

			if (Commands.Count > 0) {
				this.MakeValid();
			}

			return;
		}

		public void SendToUser(int ID, string Message)
		{
			this.SetResult(new Command {
				Contents = Message,
				ID = ID,
				Type = CommandTypes.SEND
			});

			return;
		}
	}
}
