using FW.Core;
using FW.Core.Models;

namespace FW.Game.Comms
{
	public class DoOoc : ActionBase
	{
		public DoOoc()
			: base("ooc", "ooc <message>", "Send a public 'out of character' message")
		{
			return;
		}


		public override void Act(Command Cmd, PlayerPC Player, TickDispatch Dispatch)
		{
			if (Cmd.Body.ToLower() == "ooc") {
				return;
			}

			string msg = $"`b[`yOOC`b] `y{Player.Name}: `w{Cmd.Body}`n`n";

			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC) {
					Dispatch.SendToUser(p.Key, msg);
				}
			}

			return;
		}
	}
}
