using System;
using System.Collections.Generic;
using System.Text;

namespace FW.Game
{
	public class State
	{
		public int CurrentUserID;
		public Dictionary<int, Players.Player> Players { get; set; }
		public Dictionary<int, int> PlayerSocketLookup { get; set; }


		public int AddPlayer(Players.Player Player)
		{
			this.CurrentUserID += 1;

			Player.ID = this.CurrentUserID;
			this.Players.Add(this.CurrentUserID, Player);

			if (Player is Players.PlayerPC) {
				this.PlayerSocketLookup.Add(((Players.PlayerPC)Player).SocketID, Player.ID);
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

		public void RemovePlayer(int ID)
		{
			if (!this.Players.ContainsKey(ID)) {
				return;
			}

			if (this.Players[ID] is Players.PlayerPC) {
				this.PlayerSocketLookup.Remove(((Players.PlayerPC)this.Players[ID]).SocketID);
			}

			this.Players.Remove(ID);

			return;
		}
	}
}
