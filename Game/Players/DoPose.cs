using System.Collections.Generic;

using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.Players
{
	public class DoPose : ActionBase
	{
		public DoPose(Logger Logger)
			: base(new Action[6] {
				new Action("stand", "stand", ""),
				new Action("sit", "sit", ""),
				new Action("rest", "rest", ""),
				new Action("crouch", "crouch", ""),
				new Action("turtle", "turtle", "", Mortalities.Mortal, false),
				new Action("sleep", "sleep", "")
			}, Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			var poseDescs = new Dictionary<Poses, System.Tuple<string, string>>() {
				{ Poses.Standing,  new System.Tuple<string, string>("stands up.`n", "You stand up.`n") },
				{ Poses.Sitting,   new System.Tuple<string, string>("sits down.`n", "You sit down.`n") },
				{ Poses.Laying,    new System.Tuple<string, string>("lays down to rest.`n", "You lie down to rest.`n") },
				{ Poses.Crouching, new System.Tuple<string, string>("crouches down.`n", "You crouch down.`n") },
				{ Poses.Turtling,  new System.Tuple<string, string>("curls down into a ball with their back arched, looking oddly like a turtle.`n", "You curl down into a ball and arch your back, doing your best impression of a turtle.`n") },
				{ Poses.Sleeping,  new System.Tuple<string, string>("lays down and closes their eyes to sleep.`n", "You lie down and close your eyes so you can fall asleep.`n") }
			};

			var newPose = Poses.Standing;

			switch (Cmd.Body.ToLower()) {
				case "stand":
					break;
				case "sit":
					newPose = Poses.Sitting;
					break;
				case "rest":
					newPose = Poses.Laying;
					break;
				case "crouch":
					newPose = Poses.Crouching;
					break;
				case "turtle":
					newPose = Poses.Turtling;
					break;
				case "sleep":
					newPose = Poses.Sleeping;
					break;
				default:
					return;
			}

			if (newPose == Player.Pose) {
				Dispatch.SendToUser(Player.Vnum, "You can't do that anymore than you already are!`n");

				return;
			}

			Player.Pose = newPose;

			foreach (var p in Dispatch.State.Players) {
				if (p.Value.Location.Vnum == Player.Location.Vnum) {
					Dispatch.SendToUser(p.Value.Vnum, (p.Value.Vnum == Player.Vnum) ? poseDescs[newPose].Item2 : $"{Player.Name} {poseDescs[newPose].Item1}");
				}
			}

			return;
		}
	}
}
