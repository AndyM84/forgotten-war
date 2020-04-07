using System.Numerics;

using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.Players
{
	public class PlayersNode : GameNode
	{
		public PlayersNode(ref Logger Logger)
			: base("PlayersNode", "1.0", ref Logger)
		{
			this.Log(LogLevels.DEBUG, "Initialized game PLAYERS node");

			return;
		}


		public override void Process(ref object Sender, ref TickDispatch Dispatch)
		{
			foreach (var c in Dispatch.Commands) {
				if (c.Type == CommandTypes.CONNECTED) {
					var tmp = new Character();
					tmp.ConnectionState = ConnectionStates.NamePrompt;
					tmp.Name = "NewUser" + Dispatch.State.CurrentUserID;
					tmp.ShowColor = true;
					tmp.SocketID = c.ID;
					tmp.Location = new Location(new Vector3(0.0f, 0.0f, 0.0f), 0);

					tmp.Vnum = Dispatch.State.AddPlayer(tmp);
					Dispatch.SendToUser(c.ID, "Welcome to Forgotten War!\n\n", true);
					Dispatch.SendToUser(c.ID, "What is your name? ", true);
				} else if (c.Type == CommandTypes.RECEIVED) {
					var player = Dispatch.State.GetPlayerBySocketID(c.ID);

					if (player == null) {
						continue;
					}

					if (player.ConnectionState == ConnectionStates.NamePrompt) {
						player.Name = c.Body;
						player.ConnectionState = ConnectionStates.ColorPrompt;

						Dispatch.SendToUser(player.Vnum, $"Hi there, {player.Name}, do you want to use `gc`bo`rl`co`yr`0 (Y/n)? ");
					} else if (player.ConnectionState == ConnectionStates.ColorPrompt) {
						player.ConnectionState = ConnectionStates.Connected;

						if (c.Body.ToLower() == "n") {
							player.ShowColor = false;
						}

						foreach (var p in Dispatch.State.Players) {
							Dispatch.SendToUser(p.Value.Vnum, $"`b[`yINFO`b]`0 `c{player.Name}`0 just joined!\n\n");
						}
					}
				} else if (c.Type == CommandTypes.DISCONNECTED) {
					var player = Dispatch.State.GetPlayerBySocketID(c.ID);

					if (player == null) {
						continue;
					}

					foreach (var p in Dispatch.State.Players) {
						if (Dispatch.State.GetPlayerIDBySocketID(c.ID) != p.Value.Vnum && player.ConnectionState == ConnectionStates.Connected) {
							Dispatch.SendToUser(p.Value.Vnum, $"`b[`yINFO`b]`0 `c{player.Name}`0 has disconnected!\n\n");
						}
					}

					Dispatch.State.RemovePlayer(player.Vnum);
				}
			}

			return;
		}
	}
}
