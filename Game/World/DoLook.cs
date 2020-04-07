using System.Collections.Generic;
using System.Text;

using FW.Core;
using FW.Core.Models;

namespace FW.Game.World
{
	public class DoLook : ActionBase
	{
		public DoLook()
			: base("look", "look", "Displays details on the currently occupied room for a character")
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			var output = Utilities.GetRoomOutput(Player.Location.Vnum, Player, Dispatch);

			if (!string.IsNullOrWhiteSpace(output)) {
				Dispatch.SendToUser(Player.Vnum, output);
			}

			return;
		}
	}
}
