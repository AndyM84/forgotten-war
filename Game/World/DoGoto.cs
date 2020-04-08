using System;

using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.World
{
	public class DoGoto : ActionBase
	{
		public DoGoto(Logger Logger)
			: base("goto", "goto", "Allows an administrator to change their location without having to travel", Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			if (Player.Mortality == Mortalities.Mortal) {
				return;
			}

			try {
				var vnum = Convert.ToInt32(Cmd.Body);

				if (!Dispatch.State.Rooms.ContainsKey(vnum)) {
					this.Log(LogLevels.WARNING, $"Invalid VNUM provided for GOTO by user '{Player.Name}': {vnum}");
					Dispatch.SendToUser(Player.Vnum, "You must enter a valid room number`n");

					return;
				}

				Player.Location.Vnum = vnum;
				var output = Utilities.GetRoomOutput(vnum, Player, Dispatch);

				if (!string.IsNullOrWhiteSpace(output)) {
					Dispatch.SendToUser(Player.Vnum, output);
				}
			} catch (FormatException) {
				this.Log(LogLevels.ERROR, $"Failed to perform GOTO for user '{Player.Name}', bad VNUM format: {Cmd.Body}");
				Dispatch.SendToUser(Player.Vnum, "You must enter a valid room number`n");

				return;
			}

			return;
		}
	}
}
