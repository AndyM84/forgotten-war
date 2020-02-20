using System;
using System.Collections.Generic;
using System.Text;

using FW.Core;
using Stoic.Log;

namespace FW.Game.Players
{
	public class PlayersNode : GameNode
	{
		protected Dictionary<string, Action<Command, PlayerPC, TickDispatch>> _Commands;


		public PlayersNode(ref Logger Logger)
			: base("PlayersNode", "1.0", ref Logger)
		{
			this.Log(LogLevels.DEBUG, "Initialized game PLAYERS node");
			this._Commands = new Dictionary<string, Action<Command, PlayerPC, TickDispatch>> {
				{ "who", this.Cmd_DoWho },
				{ "quit", this.Cmd_DoQuit }
			};

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
					var player = (PlayerPC)Dispatch.State.Players[Dispatch.State.GetPlayerIDBySocketID(c.ID)];

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
					} else {
						var cmd = c.Prefix.ToLower();

						if (this._Commands.ContainsKey(cmd)) {
							this._Commands[cmd](c, player, Dispatch);
						}
					}
				} else if (c.Type == CommandTypes.DISCONNECTED) {
					var player = (PlayerPC)Dispatch.State.Players[Dispatch.State.GetPlayerIDBySocketID(c.ID)];

					foreach (var p in Dispatch.State.Players) {
						if (p.Value is PlayerPC && Dispatch.State.GetPlayerIDBySocketID(c.ID) != p.Value.ID) {
							Dispatch.SendToUser(p.Value.ID, $"`b[`yINFO`b]`0 `c{player.Name}`0 has disconnected!\n\n");
						}
					}

					Dispatch.State.RemovePlayer(player.ID);
				}
			}

			return;
		}

		protected void Cmd_DoWho(Command Command, PlayerPC Player, TickDispatch Dispatch)
		{
			StringBuilder sb = new StringBuilder("`n=== Online Players ===`n");

			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC) {
					sb.Append("`b---`0 ");
					sb.Append(p.Value.Name);

					if (p.Value.ID == Player.ID) {
						sb.Append(" `w(YOU)`0");
					}

					sb.Append("`n");
				}
			}

			sb.Append("======================`n`n");
			Dispatch.SendToUser(Player.ID, sb.ToString());

			return;
		}

		protected void Cmd_DoQuit(Command Command, PlayerPC Player, TickDispatch Dispatch)
		{
			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC && Dispatch.State.GetPlayerIDBySocketID(Command.ID) != p.Value.ID) {
					Dispatch.SendToUser(p.Value.ID, $"`b[`yINFO`b]`0 `c{Player.Name}`0 has disconnected!\n\n");
				}
			}

			Dispatch.SendToUser(Player.ID, $"`n`nThanks for playing, {Player.Name}!`n`n");
			Dispatch.DisconnectUser(Player.ID);

			Dispatch.State.RemovePlayer(Player.ID);

			return;
		}
	}
}
