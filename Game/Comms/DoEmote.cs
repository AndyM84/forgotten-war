using FW.Core;
using FW.Core.Models;

namespace FW.Game.Comms
{
	public class DoEmote : ActionBase
	{
		public DoEmote()
			: base("emote", "emote <rp action>", "Send a dynamic role-play action to the current room")
		{
			return;
		}


		public override void Act(Command Cmd, PlayerPC Player, TickDispatch Dispatch)
		{
			if (Cmd.Body.ToLower() == "emote") {
				return;
			}

			string msg = $"`g{Player.Name}";

			if (!Cmd.Body.StartsWith("'")) {
				msg += " ";
			}

			msg += $"{Cmd.Body}`0`n`n";

			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC && p.Value.VnumLocation == Player.VnumLocation) {
					Dispatch.SendToUser(p.Key, msg);
				}
			}

			return;
		}
	}
}
