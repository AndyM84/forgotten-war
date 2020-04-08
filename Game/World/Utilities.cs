using System.Collections.Generic;
using System.Text;

using FW.Core;
using FW.Core.Models;

namespace FW.Game.World
{
	public class Utilities
	{
		public static Room GetNextRoomInDirection(int Vnum, Directions Direction, Character Player, TickDispatch Dispatch)
		{
			if (!Dispatch.State.Rooms.ContainsKey(Vnum)) {
				return null;
			}

			var current = Dispatch.State.Rooms[Vnum];

			if (current.Exits.Count < 1 || !current.Exits.ContainsKey(Direction)) {
				return null;
			}

			var exit = current.Exits[Direction];

			if (!Dispatch.State.Rooms.ContainsKey(exit.Destination)) {
				return null;
			}

			return Dispatch.State.Rooms[exit.Destination];
		}

		public static string GetRoomOutput(int Vnum, Character Player, TickDispatch Dispatch)
		{
			if (!Dispatch.State.Rooms.ContainsKey(Vnum)) {
				return string.Empty;
			}

			StringBuilder sb = new StringBuilder();
			List<string> inRoom = new List<string>();
			Room room = Dispatch.State.Rooms[Vnum];

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

			return sb.ToString();
		}

		public static void DoRoomJoin(Character Player, TickDispatch Dispatch)
		{
			string text = $"{Player.Name} has entered the room.`n";

			foreach (var p in Dispatch.State.Players) {
				// We can put visibility checks and such in here

				if (p.Value.Vnum != Player.Vnum) {
					Dispatch.SendToUser(p.Value.Vnum, text);
				}
			}

			return;
		}
	}
}
