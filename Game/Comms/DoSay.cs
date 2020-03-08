using FW.Core;
using FW.Core.Models;

namespace FW.Game.Comms
{
	public class DoSay : ActionBase
	{
		public DoSay()
			: base("say", "say <message>", "Broadcast a spoken message to the current room")
		{
			return;
		}


		public override void Act(Command Cmd, PlayerPC Player, TickDispatch Dispatch)
		{
			if (Cmd.Body.ToLower() == "say") {
				return;
			}

			string msg = $"`g{Player.Name} says, \"";

			msg += $"{Cmd.Body}\"`0`n`n";

			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC && p.Value.VnumLocation == Player.VnumLocation) {
					Dispatch.SendToUser(p.Key, msg);
				}
			}

			return;
		}
	}
}
