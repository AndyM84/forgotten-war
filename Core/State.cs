using System.Collections.Generic;

using FW.Core.Models;

namespace FW.Core
{
	public class State
	{
		public string Branch { get; set; }
		public string Commit { get; set; }
		public Config Config { get; set; }
		public int CurrentUserID;
		public Dictionary<int, Character> Players { get; set; }
		public Dictionary<int, int> PlayerSocketLookup { get; set; }
		public Dictionary<int, Room> Rooms { get; set; }
		public string Version { get; set; }


		public State()
		{
			this.Branch             = "master";
			this.Commit             = "000000";
			this.Config             = new Config();
			this.CurrentUserID      = 0;
			this.Players            = new Dictionary<int, Character>();
			this.PlayerSocketLookup = new Dictionary<int, int>();
			this.Rooms              = new Dictionary<int, Room>();
			this.Version            = "0.0.0.0";

			return;
		}


		public int AddPlayer(Character Player)
		{
			this.CurrentUserID += 1;

			Player.Vnum = this.CurrentUserID * -1;
			this.Players.Add(Player.Vnum, Player);
			this.PlayerSocketLookup.Add(Player.SocketID, Player.Vnum);

			return Player.Vnum;
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

		public void SetPlayerVnum(int NewVnum, int OldVnum)
		{
			if (NewVnum < 0) {
				return;
			}

			this.Players.Add(NewVnum, this.Players[OldVnum]);
			this.Players.Remove(OldVnum);

			this.PlayerSocketLookup[this.Players[NewVnum].SocketID] = NewVnum;

			return;
		}
	}
}
