using System.Collections.Generic;

using FW.Core.Models;

namespace FW.Core
{
	public class State
	{
		public int CurrentUserID;
		public Dictionary<int, Character> Players { get; set; }
		public Dictionary<int, int> PlayerSocketLookup { get; set; }


		public int AddPlayer(Character Player)
		{
			this.CurrentUserID += 1;

			Player.Vnum = this.CurrentUserID;
			this.Players.Add(this.CurrentUserID, Player);
			this.PlayerSocketLookup.Add(Player.SocketID, Player.Vnum);

			return this.CurrentUserID;
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
