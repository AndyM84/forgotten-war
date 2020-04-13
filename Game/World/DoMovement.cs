using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.World
{
	public class DoMovement : ActionBase
	{
		public DoMovement(Logger Logger)
			: base(new Action[7] {
				new Action("north", "north", "Attempts to move a character north through an available exit"),
				new Action("south", "south", "Attempts to move a character south through an available exit"),
				new Action("east", "east", "Attempts to move a character east through an available exit"),
				new Action("west", "west", "Attempts to move a character west through an available exit"),
				new Action("up", "up", "Attempts to move a character up through an available exit"),
				new Action("down", "down", "Attempts to move a character down through an available exit"),
				new Action("goto", "goto", "Allows an imm to change their location without having to travel", Mortalities.Immortal)
			}, Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			if (Cmd.Prefix.ToLower() == "goto") {
				this.DoGoto(Cmd, Player, Dispatch);

				return;
			}

			switch (Player.Pose) {
				case Poses.Sitting:
				case Poses.Laying:
				case Poses.Sleeping:
					Dispatch.SendToUser(Player.Vnum, "You can't move if you are resting!`n");

					return;
			}

			var direction = Directions.North;

			switch (Cmd.Body) {
				case "south":
					direction = Directions.South;
					break;
				case "east":
					direction = Directions.East;
					break;
				case "west":
					direction = Directions.West;
					break;
				case "up":
					direction = Directions.Up;
					break;
				case "down":
					direction = Directions.Down;
					break;
			}

			var destination = Utilities.GetNextRoomInDirection(Player.Location.Vnum, direction, Player, Dispatch);

			if (destination == null) {
				return;
			}

			var oldRoom = Player.Location.Vnum;
			Player.Location.Vnum = destination.Vnum;

			var output = Utilities.GetRoomOutput(Player.Location.Vnum, Player, Dispatch);

			if (!string.IsNullOrWhiteSpace(output)) {
				Dispatch.SendToUser(Player.Vnum, output);
				this.DoRoomJoin(Player, Dispatch);
				this.DoRoomLeave(oldRoom, Player, Dispatch);
			}

			return;
		}

		protected void DoGoto(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			if (Player.Mortality == Mortalities.Mortal) {
				return;
			}

			try {
				var vnum = System.Convert.ToInt32(Cmd.Body);

				if (!Dispatch.State.Rooms.ContainsKey(vnum)) {
					this.Log(LogLevels.WARNING, $"Invalid VNUM provided for GOTO by user '{Player.Name}': {vnum}");
					Dispatch.SendToUser(Player.Vnum, "You must enter a valid room number`n");

					return;
				}

				var oldRoom = Player.Location.Vnum;
				Player.Location.Vnum = vnum;
				var output = Utilities.GetRoomOutput(vnum, Player, Dispatch);

				if (!string.IsNullOrWhiteSpace(output)) {
					Dispatch.SendToUser(Player.Vnum, output);
					this.DoRoomJoin(Player, Dispatch);
					this.DoRoomLeave(oldRoom, Player, Dispatch);
				}
			} catch (System.FormatException) {
				this.Log(LogLevels.ERROR, $"Failed to perform GOTO for user '{Player.Name}', bad VNUM format: {Cmd.Body}");
				Dispatch.SendToUser(Player.Vnum, "You must enter a valid room number`n");

				return;
			}

			return;
		}

		protected void DoRoomJoin(Character Player, TickDispatch Dispatch)
		{
			string text = $"{Player.Name} has entered the room.`n";

			if (Player.Pose == Poses.Turtling) {
				text = $"{Player.Name} enters the room slowly, crab-walking while they impersonate a turtle.`n";
			}

			foreach (var p in Dispatch.State.Players) {
				// We can put visibility checks and such in here

				if (p.Value.Location.Vnum == Player.Location.Vnum && p.Value.Vnum != Player.Vnum) {
					Dispatch.SendToUser(p.Value.Vnum, text);
				}
			}

			return;
		}

		protected void DoRoomLeave(int Vnum, Character Player, TickDispatch Dispatch)
		{
			string text = $"{Player.Name} has left the room.`n";

			if (Player.Pose == Poses.Turtling) {
				text = $"{Player.Name} leaves the room slowly, crab-walking while they impersonate a turtle.`n";
			}

			foreach (var p in Dispatch.State.Players) {
				// We can put visibility checks and such in here

				if (p.Value.Location.Vnum == Vnum && p.Value.Vnum != Player.Vnum) {
					Dispatch.SendToUser(p.Value.Vnum, text);
				}
			}

			return;
		}
	}
}
