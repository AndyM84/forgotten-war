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
			if (!Dispatch.State.Rooms.ContainsKey(Player.Location.Vnum)) {
				return;
			}

			StringBuilder sb = new StringBuilder();
			List<string> inRoom = new List<string>();
			Room room = Dispatch.State.Rooms[Player.Location.Vnum];

			sb.Append($"{room.Name}`n");
			sb.Append($"{room.Description}`n`n");

			foreach (var p in Dispatch.State.Players) {
				if (p.Value.Location.Vnum == room.Vnum && p.Value.Vnum != Player.Vnum) {
					inRoom.Add(p.Value.Name);
				}
			}

			if (inRoom.Count > 0) {
				sb.Append("You see: ");
				sb.Append(string.Join(", ", inRoom.ToArray()));
				sb.Append("`n");
			}

			if (room.Exits.Count > 0) {
				sb.Append("Exits: ");
				sb.Append(string.Join(", ", room.Exits.Keys));
				sb.Append("`n");
			}

			Dispatch.SendToUser(Player.Vnum, sb.ToString());

			return;
		}
	}
}
