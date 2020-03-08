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
					var tmp = new PlayerPC {
						ConnectionState = ConnectionStates.NamePrompt,
						CoordLocation = new System.Numerics.Vector3(0f, 0f, 0f),
						ID = 0,
						Name = "NewUser" + Dispatch.State.CurrentUserID,
						ShowColor = true,
						SocketID = c.ID,
						VnumLocation = 0f
					};

					tmp.ID = Dispatch.State.AddPlayer(tmp);
					Dispatch.SendToUser(c.ID, "Welcome to Forgotten War!\n\n", true);
					Dispatch.SendToUser(c.ID, "What is your name? ", true);
				} else if (c.Type == CommandTypes.RECEIVED) {
					var player = (PlayerPC)Dispatch.State.GetPlayerBySocketID(c.ID);

					if (player == null) {
						continue;
					}

					if (player.ConnectionState == ConnectionStates.NamePrompt) {
						player.Name = c.Body;
						player.ConnectionState = ConnectionStates.ColorPrompt;

						Dispatch.SendToUser(player.ID, $"Hi there, {player.Name}, do you want to use `gc`bo`rl`co`yr`0 (Y/n)? ");
					} else if (player.ConnectionState == ConnectionStates.ColorPrompt) {
						player.ConnectionState = ConnectionStates.Connected;

						if (c.Body.ToLower() == "n") {
							player.ShowColor = false;
						}

						foreach (var p in Dispatch.State.Players) {
							if (p.Value is PlayerPC) {
								Dispatch.SendToUser(p.Value.ID, $"`b[`yINFO`b]`0 `c{player.Name}`0 just joined!\n\n");
							}
						}
					}
				} else if (c.Type == CommandTypes.DISCONNECTED) {
					var player = (PlayerPC)Dispatch.State.GetPlayerBySocketID(c.ID);

					if (player == null) {
						continue;
					}

					foreach (var p in Dispatch.State.Players) {
						if (p.Value is PlayerPC && Dispatch.State.GetPlayerIDBySocketID(c.ID) != p.Value.ID && player.ConnectionState == ConnectionStates.Connected) {
							Dispatch.SendToUser(p.Value.ID, $"`b[`yINFO`b]`0 `c{player.Name}`0 has disconnected!\n\n");
						}
					}

					Dispatch.State.RemovePlayer(player.ID);
				}
			}

			return;
		}
	}
}
