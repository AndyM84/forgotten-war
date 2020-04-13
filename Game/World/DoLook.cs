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
			var poseDescs = new Dictionary<Poses, System.Tuple<string, string>>() {
				{ Poses.Standing,  new System.Tuple<string, string>("is standing here.`n", "You are standing here.`n") },
				{ Poses.Sitting,   new System.Tuple<string, string>("is sitting here.`n", "You are sitting here.`n") },
				{ Poses.Laying,    new System.Tuple<string, string>("is laying here.`n", "You are laying here.`n") },
				{ Poses.Crouching, new System.Tuple<string, string>("is crouching here.`n", "You are crouching here.`n") },
				{ Poses.Turtling,  new System.Tuple<string, string>("is curled up in a ball with their back arched, looking oddly like a turtle.`n", "You are curled up in a ball with your back arched, looking very much like a turtle.`n") },
				{ Poses.Sleeping,  new System.Tuple<string, string>("is sleeping here.`n", "You are sleeping here.`n") }
			};

			// TODO: Order to build list is as follows: objects, mobs, characters
			// Everything should get a 'short' version and then a 'long' version so we can look at specific items if there are dupes

			if (lCmd == Player.Name.ToLower() || lCmd == "me") {
				Dispatch.SendToUser(Player.Vnum, poseDescs[Player.Pose].Item2);

				return;
			}

			foreach (var p in Dispatch.State.Players) {
				if (p.Value.Location.Vnum == room.Vnum && p.Value.Vnum != Player.Vnum) {
					inRoom.Add(p.Value.Name.ToLower(), $"{p.Value.Name} {poseDescs[p.Value.Pose].Item1}");
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
