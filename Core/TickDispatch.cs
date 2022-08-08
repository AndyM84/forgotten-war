using System.Collections.Generic;

using FW.Core.Models;
using Stoic.Chain;

namespace FW.Core
{
	public class TickDispatch : DispatchBase<Command, List<Command>>
	{
		protected static List<PromptTokenBase> _PromptTokens = new();

		protected List<Command> _Commands = new();
		public State State;

		public List<Command> Commands { get { return new List<Command>(this._Commands); } }


		public static string AttemptColor(string Message, bool AllowColor)
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

		public string AttemptPrompt(string Message, Character Player)
		{
			var prompt = Player.Prompt;

			foreach (var p in TickDispatch._PromptTokens) {
				foreach (var tok in p.PromptTokens) {
					if (prompt.Contains(tok.Token) && tok.MinMortality <= Player.Mortality) {
						prompt = prompt.Replace(tok.Token, p.GenerateSegment(Player, this.State));
					}
				}
			}

			return Message + prompt;
		}

		public void DisconnectUser(int ID, bool AsSocket = false)
		{
			var sId = ID;

			if (!AsSocket && !this.State.Players.ContainsKey(ID)) {
				return;
			}

			if (!AsSocket) {
				sId = this.State.Players[ID].SocketID;
			}

			this.SetResult(new Command {
				Contents = string.Empty,
				ID = sId,
				Type = CommandTypes.DISCONNECTED
			});

			return;
		}

		public override void Initialize()
		{
			throw new System.NotImplementedException();
		}

		public void Initialize(State State, List<Command> Commands)
		{
			this.State     = State;
			this._Commands = new List<Command>(Commands);

			this.MakeStateful();

			if (Commands.Count > 0) {
				this.MakeValid();
			}

			if (TickDispatch._PromptTokens.Count < 1) {
				this.Reset();
			}

			return;
		}

		public void Reset()
		{
			this._Commands.Clear();
			TickDispatch._PromptTokens.Clear();

			System.Type basePromptType = typeof(PromptTokenBase);
			TickDispatch._PromptTokens = new List<PromptTokenBase>();

			foreach (var asm in System.AppDomain.CurrentDomain.GetAssemblies()) {
				foreach (var t in asm.GetTypes()) {
					if (basePromptType.IsAssignableFrom(t) && !t.IsAbstract) {
						TickDispatch._PromptTokens.Add((PromptTokenBase)System.Activator.CreateInstance(t));
					}
				}
			}

			return;
		}

		public void SendToUser(int ID, string Message, bool AsSocket = false)
		{
			var sId = ID;

			if (!AsSocket && !this.State.Players.ContainsKey(ID)) {
				return;
			}

			if (!AsSocket) {
				sId = (this.State.Players[ID]).SocketID;

				Message = this.AttemptPrompt(Message, this.State.Players[ID]);
				Message = AttemptColor($"`n{Message}", this.State.Players[ID].ShowColor);
			} else {
				Message = AttemptColor($"`n{Message}", true);
			}

			this.SetResult(new Command {
				Contents = Message,
				ID = sId,
				Type = CommandTypes.SEND
			});

			return;
		}
	}
}
