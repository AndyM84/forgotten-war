using FW.Core;
using FW.Core.Models;

namespace FW.Game.World
{
	public class DoEast : ActionBase
	{
		public DoEast()
			: base("east", "east", "Attempts to move a character east through an available exit")
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			var destination = Utilities.GetNextRoomInDirection(Player.Location.Vnum, Directions.East, Player, Dispatch);

			if (destination == null) {
				return;
			}

			Player.Location.Vnum = destination.Vnum;

			var output = Utilities.GetRoomOutput(Player.Location.Vnum, Player, Dispatch);

			if (!string.IsNullOrWhiteSpace(output)) {
				Dispatch.SendToUser(Player.Vnum, output);
			}

			return;
		}
	}
}
