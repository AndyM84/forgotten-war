using System.Collections.Generic;

using FW.Core.Models;

namespace FW.Game
{
	public class State
	{
		public int CurrentUserID;
		public Dictionary<int, Player> Players { get; set; }
		public Dictionary<int, int> PlayerSocketLookup { get; set; }


		public int AddPlayer(Player Player)
		{
			this.CurrentUserID += 1;

			Player.ID = this.CurrentUserID;
			this.Players.Add(this.CurrentUserID, Player);

			if (Player is PlayerPC) {
				this.PlayerSocketLookup.Add(((PlayerPC)Player).SocketID, Player.ID);
			}

			return this.CurrentUserID;
		}

		public int GetPlayerIDBySocketID(int ID)
		{
			if (this.PlayerSocketLookup.ContainsKey(ID)) {
				return this.PlayerSocketLookup[ID];
			}

			return 0;
		}

		public Player GetPlayerBySocketID(int ID)
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

			if (this.Players[ID] is PlayerPC) {
				this.PlayerSocketLookup.Remove(((PlayerPC)this.Players[ID]).SocketID);
			}

			this.Players.Remove(ID);

			return;
		}
	}
}
