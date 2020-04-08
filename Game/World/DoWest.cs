using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.World
{
	public class DoWest : ActionBase
	{
		public DoWest(Logger Logger)
			: base("west", "west", "Attempts to move a character west through an available exit", Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			var destination = Utilities.GetNextRoomInDirection(Player.Location.Vnum, Directions.West, Player, Dispatch);

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
