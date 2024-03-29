﻿using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.Comms
{
	public class DoEmote : ActionBase
	{
		public DoEmote(Logger Logger)
			: base("emote", "emote <rp action>", "Send a dynamic role-play action to the current room", Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			if (Cmd.Body.ToLower() == "emote") {
				return;
			}

			string msg = $"`g{Player.Name}";

			if (!Cmd.Body.StartsWith("'")) {
				msg += " ";
			}

			msg += $"{Cmd.Body}`0`n";

			foreach (var p in Dispatch.State.Players) {
				if (p.Value.Location.Vnum == Player.Location.Vnum) {
					Dispatch.SendToUser(p.Key, msg);
				}
			}

			return;
		}
	}
}
