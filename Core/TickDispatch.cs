using System.Collections.Generic;

using Stoic.Chain;

namespace FW.Core
{
	public class TickDispatch : DispatchBase<Command, List<Command>>
	{
		protected List<Command> _Commands;
		public Game.State State;

		public List<Command> Commands { get { return new List<Command>(this._Commands); } }


		public string AttemptColor(string Message, bool AllowColor)
		{
			var replacements = new Dictionary<string, string> {
				{ "0", "[0m" },
				{ "w", "[0;37m" },
				{ "W", "[1;37m" },
				{ "g", "[0;32m" },
				{ "G", "[1;32m" },
				{ "b", "[0;34m" },
				{ "B", "[1;34m" },
				{ "r", "[0;31m" },
				{ "R", "[1;31m" },
				{ "c", "[0;36m" },
				{ "C", "[1;36m" },
				{ "y", "[0;33m" },
				{ "Y", "[1;33m" },
				{ "m", "[0;35m" },
				{ "M", "[1;35m" },
				{ "k", "[0;30m" },
				{ "K", "[1;30m" }
			};

			Message = Message.Replace("``", "___``___");

			foreach (var combo in replacements) {
				if (AllowColor) {
					Message = Message.Replace($"`{combo.Key}", $"\u001b{combo.Value}");
				} else {
					Message = Message.Replace($"`{combo.Key}", "");
				}
			}

			Message = Message.Replace("___``___", "`");
			Message = Message.Replace("`n", "\n");

			return Message;
		}

		public override void Initialize()
		{
			throw new System.NotImplementedException();
		}

		public void Initialize(Game.State State, List<Command> Commands)
		{
			this._Commands = new List<Command>(Commands);
			this.State = State;

			this.MakeStateful();

			if (Commands.Count > 0) {
				this.MakeValid();
			}

			return;
		}

		public void SendToUser(int ID, string Message, bool AsSocket = false)
		{
			var sId = ID;

			if (!AsSocket && (!this.State.Players.ContainsKey(ID) || !(this.State.Players[ID] is Models.PlayerPC))) {
				return;
			}

			if (!AsSocket) {
				sId = ((Models.PlayerPC)this.State.Players[ID]).SocketID;
				Message = this.AttemptColor(Message, ((Models.PlayerPC)this.State.Players[ID]).ShowColor);
			}

			this.SetResult(new Command {
				Contents = Message,
				ID = sId,
				Type = CommandTypes.SEND
			});

			return;
		}

		public void DisconnectUser(int ID, bool AsSocket = false)
		{
			var sId = ID;

			if (!AsSocket && (!this.State.Players.ContainsKey(ID) || !(this.State.Players[ID] is Models.PlayerPC))) {
				return;
			}

			if (!AsSocket) {
				sId = ((Models.PlayerPC)this.State.Players[ID]).SocketID;
			}

			this.SetResult(new Command {
				Contents = string.Empty,
				ID = sId,
				Type = CommandTypes.DISCONNECTED
			});

			return;
		}
	}
}
