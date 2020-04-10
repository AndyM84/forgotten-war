using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.Comms
{
	public class DoOoc : ActionBase
	{
		public DoOoc(Logger Logger)
			: base("ooc", "ooc <message>", "Send a public 'out of character' message", Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			if (Cmd.Body.ToLower() == "ooc") {
				return;
			}

			string msg = $"`b[`yOOC`b] `y{Player.Name}: `w{Cmd.Body}`n`n";

			foreach (var p in Dispatch.State.Players) {
				Dispatch.SendToUser(p.Key, msg);
			}

			return;
		}
	}
}
