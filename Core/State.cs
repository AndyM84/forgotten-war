using System.Collections.Generic;

using FW.Core.Models;

namespace FW.Core
{
	public class State
	{
		public string Branch { get; set; }
		public string Commit { get; set; }
		public int CurrentUserID;
		public Dictionary<int, Character> Players { get; set; }
		public Dictionary<int, int> PlayerSocketLookup { get; set; }
		public Dictionary<int, Room> Rooms { get; set; }
		public string Version { get; set; }


		public State()
		{
			this.Branch = "master";
			this.Commit = "000000";
			this.CurrentUserID = 0;
			this.Players = new Dictionary<int, Character>();
			this.PlayerSocketLookup = new Dictionary<int, int>();
			this.Rooms = new Dictionary<int, Room>();
			this.Version = "0.0.0.0";

			return;
		}


		public int AddPlayer(Character Player)
		{
			this.CurrentUserID += 1;

			Player.Vnum = this.CurrentUserID;
			this.Players.Add(this.CurrentUserID, Player);
			this.PlayerSocketLookup.Add(Player.SocketID, Player.Vnum);

			return this.CurrentUserID;
		}

		public void AddRoom(Room room)
		{
			this.Rooms.Add(room.Vnum, room);

			return;
		}

		public int GetPlayerIDBySocketID(int ID)
		{
			if (this.PlayerSocketLookup.ContainsKey(ID)) {
				return this.PlayerSocketLookup[ID];
			}

			return 0;
		}

		public Character GetPlayerBySocketID(int ID)
		{
			if (this.PlayerSocketLookup.ContainsKey(ID)) {
				return this.Players[this.PlayerSocketLookup[ID]];
			}

			return null;
		}

		public void RemovePlayer(int ID)
		{
			if (!this.Players.ContainsKey(ID)) {
				return;
			}

			this.PlayerSocketLookup.Remove(this.Players[ID].SocketID);
			this.Players.Remove(ID);

			return;
		}
	}
}
