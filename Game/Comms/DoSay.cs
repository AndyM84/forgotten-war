﻿using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.Comms
{
	public class DoSay : ActionBase
	{
		public DoSay(Logger Logger)
			: base("say", "say <message>", "Broadcast a spoken message to the current room", Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			if (Cmd.Body.ToLower() == "say") {
				return;
			}

			string msg = $"`g{Player.Name} says, \"";

			msg += $"{Cmd.Body}`g\"`0`n";

			foreach (var p in Dispatch.State.Players) {
				if (p.Value.Location.Vnum == Player.Location.Vnum) {
					Dispatch.SendToUser(p.Key, msg);
				}
			}

			return;
		}
	}
}
