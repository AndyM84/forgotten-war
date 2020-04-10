using System.Collections.Generic;

using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.World
{
	public class DoLook : ActionBase
	{
		public DoLook(Logger Logger)
			: base("look", "look", "Displays details on the currently occupied room for a character", Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			var lCmd = Cmd.Body.ToLower();

			if (lCmd == "look") {
				var output = Utilities.GetRoomOutput(Player.Location.Vnum, Player, Dispatch);

				if (!string.IsNullOrWhiteSpace(output)) {
					Dispatch.SendToUser(Player.Vnum, output);
				}

				return;
			}

			if (!Dispatch.State.Rooms.ContainsKey(Player.Location.Vnum)) {
				return;
			}

			var inRoom = new Dictionary<string, string>();
			var room = Dispatch.State.Rooms[Player.Location.Vnum];
			var poseDescs = new Dictionary<Poses, string>() {
				{ Poses.Standing, "is standing here.`n" },
				{ Poses.Sitting, "is sitting here.`n" },
				{ Poses.Laying, "is laying here.`n" },
				{ Poses.Crouching, "is crouching here.`n" },
				{ Poses.Turtling, "is curled up in a ball with their back arched, looking oddly like a turtle.`n" },
				{ Poses.Sleeping, "is sleeping here.`n" }
			};

			// TODO: Order to build list is as follows: objects, mobs, characters
			// Everything should get a 'short' version and then a 'long' version so we can look at specific items if there are dupes

			foreach (var p in Dispatch.State.Players) {
				if (p.Value.Location.Vnum == room.Vnum && p.Value.Vnum != Player.Vnum) {
					inRoom.Add(p.Value.Name.ToLower(), $"{p.Value.Name} {poseDescs[p.Value.Pose]}");
				}
			}

			if (!inRoom.ContainsKey(lCmd)) {
				Dispatch.SendToUser(Player.Vnum, "That doesn't appear to be in the room.`n");

				return;
			}

			Dispatch.SendToUser(Player.Vnum, inRoom[lCmd]);

			return;
		}
	}
}
