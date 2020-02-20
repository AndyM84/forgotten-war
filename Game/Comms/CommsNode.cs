using System;
using System.Collections.Generic;

using FW.Core;
using FW.Game.Players;
using Stoic.Log;

namespace FW.Game.Comms
{
	public class CommsNode : GameNode
	{
		protected Dictionary<string, Action<Command, PlayerPC, TickDispatch>> _Commands;


		public CommsNode(ref Logger Logger)
			: base("CommsNode", "1.0", ref Logger)
		{
			this.Log(LogLevels.DEBUG, "Initialized game COMMS node");
			this._Commands = new Dictionary<string, Action<Command, PlayerPC, TickDispatch>> {
				{ "ooc", this.Cmd_DoOoc },
				{ "emote", this.Cmd_DoEmote },
				{ "say", this.Cmd_DoSay }
			};

			return;
		}


		public override void Process(ref object Sender, ref TickDispatch Dispatch)
		{
			foreach (var c in Dispatch.Commands) {
				if (c.Type != CommandTypes.RECEIVED) {
					continue;
				}

				var cmd = c.Prefix.ToLower();
				var player = (PlayerPC)Dispatch.State.Players[Dispatch.State.GetPlayerIDBySocketID(c.ID)];

				if (this._Commands.ContainsKey(cmd)) {
					this._Commands[cmd](c, player, Dispatch);
				}
			}

			return;
		}

		protected void Cmd_DoOoc(Command Command, PlayerPC Player, TickDispatch Dispatch)
		{
			string msg = $"`b[`yOOC`b] `y{Player.Name}: `w{Command.Body}`n`n";

			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC) {
					Dispatch.SendToUser(p.Key, msg);
				}
			}

			return;
		}

		protected void Cmd_DoEmote(Command Command, PlayerPC Player, TickDispatch Dispatch)
		{
			string msg = $"`g{Player.Name}";

			if (!Command.Body.StartsWith("'")) {
				msg += " ";
			}

			msg += $"{Command.Body}`0`n`n";

			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC && p.Value.VnumLocation == Player.VnumLocation) {
					Dispatch.SendToUser(p.Key, msg);
				}
			}

			return;
		}

		protected void Cmd_DoSay(Command Command, PlayerPC Player, TickDispatch Dispatch)
		{
			string msg = $"`g{Player.Name} says, \"";

			msg += $"{Command.Body}\"`0`n`n";

			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC && p.Value.VnumLocation == Player.VnumLocation) {
					Dispatch.SendToUser(p.Key, msg);
				}
			}

			return;
		}
	}
}
