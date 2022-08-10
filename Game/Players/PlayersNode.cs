using System;
using System.Numerics;

using Dapper;
using MySql.Data.MySqlClient;
using Stoic.Log;

using FW.Core;
using FW.Core.DbModels;
using FW.Core.Models;

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
					var tmp = new Character {
						ConnectionState = ConnectionStates.NamePrompt,
						Mortality       = Mortalities.Mortal,
						Name            = "NewUser" + Dispatch.State.CurrentUserID,
						Prompt          = "`n< %stime% >`n",
						ShowColor       = true,
						SocketID        = c.ID,
						Location        = new Location(new Vector3(0.0f, 0.0f, 0.0f), 1)
					};

					tmp.Vnum = Dispatch.State.AddPlayer(tmp);
					Dispatch.SendToUser(c.ID, "Welcome to Forgotten War!`n", true);
					Dispatch.SendToUser(c.ID, "What is your name? ", true);
				} else if (c.Type == CommandTypes.RECEIVED) {
					var player = Dispatch.State.GetPlayerBySocketID(c.ID);

					if (player == null) {
						continue;
					}

					if (player.ConnectionState == ConnectionStates.NamePrompt) {
						bool goodName = true;

						if (c.Body.Contains(' ')) {
							goodName = false;
						}

						if (c.Body.Length > 24) {
							goodName = false;
						}

						foreach (var p in Dispatch.State.Players) {
							if (c.Body.ToLower() == p.Value.Name.ToLower()) {
								Dispatch.SendToUser(player.SocketID, "Sorry, somebody is already using that name, please try again: ", true);

								goodName = false;

								break;
							}
						}

						if (!goodName) {
							continue;
						}

						bool existingUser = false;
						PlayerChar tmp    = new PlayerChar();
						var nameLower     = c.Body.ToLower();

						try {
							using var conn = new MySqlConnection(Dispatch.State.Config.DbDsn);
							var pChar = conn.QuerySingleOrDefault<PlayerChar>("SELECT * FROM `PlayerChar` WHERE `NameLowered` = @Name", new { Name = nameLower });

							if (pChar != null) {
								existingUser = true;
								tmp          = pChar;
							}
						} catch (MySqlException mex) {
							Log(LogLevels.ERROR, $"Error searching for player character: {mex.Number} - {mex.Message}");
						}

						if (existingUser) {
							Dispatch.State.SetPlayerVnum(tmp.Vnum, player.Vnum);
							player.HydrateFromPlayerChar(tmp);

							if (tmp.PasswordNeedsChanged) {
								player.ConnectionState = ConnectionStates.ResetPassword;
								Dispatch.SendToUser(player.SocketID, "Enter a New Password: ", true);
							} else {
								player.ConnectionState = ConnectionStates.PasswordPrompt;
								Dispatch.SendToUser(player.SocketID, "Enter Your Password: ", true);
							}

							continue;
						}
						
						player.Name = c.Body;
						player.ConnectionState = ConnectionStates.NewPassword;

						Dispatch.SendToUser(player.SocketID, "Create Your Password: ", true);
					} else if (player.ConnectionState == ConnectionStates.NewPassword) {
						if (c.Body.Length > 56 || c.Body.Length < 3) {
							Dispatch.SendToUser(player.SocketID, "Password must be between 6 and 56 characters long..`n", true);
							Dispatch.SendToUser(player.SocketID, "Enter a New Password: ", true);

							continue;
						}

						try {
							var newPass    = BCrypt.Net.BCrypt.EnhancedHashPassword(c.Body);
							using var conn = new MySqlConnection(Dispatch.State.Config.DbDsn);
							
							conn.Execute(@"INSERT INTO `PlayerChar` (`Created`, `Prompt`, `Birthdate`, `Name`, `NameLowered`, `Password`, `PosVnum`) VALUES (NOW(), '`n< %stime% >`n', NOW(), @Name, @NameLowered, @Password, 1)", new {
								Name        = player.Name,
								NameLowered = player.Name.ToLower(),
								Password    = newPass
							});

							var vnum = conn.QueryFirst<int>("SELECT Vnum FROM `PlayerChar` WHERE `NameLowered` = @NameLowered", new { NameLowered = player.Name.ToLower() });							
							Dispatch.State.SetPlayerVnum(vnum, player.Vnum);
							player.Vnum = vnum;

							player.ConnectionState = ConnectionStates.ColorPrompt;
							Dispatch.SendToUser(player.SocketID, $"Hi there, {player.Name}, do you want to use `gc`bo`rl`co`yr`0 (y/n)? ", true);
						} catch (MySqlException mex) {
							Log(LogLevels.ERROR, $"Error searching for player character: {mex.Number} - {mex.Message}");
						}
					} else if (player.ConnectionState == ConnectionStates.ResetPassword) {
						if (c.Body.Length > 56 || c.Body.Length < 3) {
							Dispatch.SendToUser(player.SocketID, "Password must be between 6 and 56 characters long..`n", true);
							Dispatch.SendToUser(player.SocketID, "Enter a New Password: ", true);

							continue;
						}

						try {
							var newPass    = BCrypt.Net.BCrypt.EnhancedHashPassword(c.Body);
							using var conn = new MySqlConnection(Dispatch.State.Config.DbDsn);
							
							conn.Execute("UPDATE `PlayerChar` SET `Password` = @Password, `PasswordNeedsChanged` = 0 WHERE `Vnum` = @Vnum LIMIT 1", new { Password = newPass, player.Vnum });

							player.ConnectionState = ConnectionStates.ColorPrompt;
							Dispatch.SendToUser(player.SocketID, $"Hi there, {player.Name}, do you want to use `gc`bo`rl`co`yr`0 (y/n)? ", true);
						} catch (MySqlException mex) {
							Log(LogLevels.ERROR, $"Error searching for player character: {mex.Number} - {mex.Message}");
						}
					} else if (player.ConnectionState == ConnectionStates.PasswordPrompt) {
						try {
							using var conn = new MySqlConnection(Dispatch.State.Config.DbDsn);
							var pass = conn.QuerySingleOrDefault<string>("SELECT `Password` FROM `PlayerChar` WHERE `Vnum` = @Vnum", new { player.Vnum });

							if (string.IsNullOrWhiteSpace(pass)) {
								Log(LogLevels.WARNING, $"Couldn't find password for pchar #{player.Vnum} ({player.Name})");

								Dispatch.DisconnectUser(player.SocketID, true);
								Dispatch.State.RemovePlayer(player.Vnum);

								continue;
							}

							if (BCrypt.Net.BCrypt.EnhancedVerify(c.Body, pass)) {
								player.ConnectionState = ConnectionStates.ColorPrompt;
								Dispatch.SendToUser(player.SocketID, $"Hi there, {player.Name}, do you want to use `gc`bo`rl`co`yr`0 (y/n)? ", true);

								continue;
							}

							Dispatch.SendToUser(player.SocketID, "Incorrect Password..`n", true);
							Dispatch.SendToUser(player.SocketID, "Enter Your Password: ", true);
						} catch (MySqlException mex) {
							Log(LogLevels.ERROR, $"Error searching for player character: {mex.Number} - {mex.Message}");
						}
					} else if (player.ConnectionState == ConnectionStates.ColorPrompt) {
						player.Connected       = DateTime.UtcNow;
						player.ConnectionState = ConnectionStates.Connected;

						if (c.Body.ToLower() == "n" || c.Body.ToLower() == "no") {
							player.ShowColor = false;
						}

						foreach (var p in Dispatch.State.Players) {
							Dispatch.SendToUser(p.Value.Vnum, $"`b[`yINFO`b]`0 `c{player.Name}`0 just joined!`n");
						}

						var output = World.Utilities.GetRoomOutput(player.Location.Vnum, player, Dispatch);

						if (!string.IsNullOrWhiteSpace(output)) {
							Dispatch.SendToUser(player.Vnum, output);
						}
					}
				} else if (c.Type == CommandTypes.DISCONNECTED) {
					var player = Dispatch.State.GetPlayerBySocketID(c.ID);

					if (player == null) {
						continue;
					}

					try {
						using var conn = new MySqlConnection(Dispatch.State.Config.DbDsn);
						player.SaveToDb(conn, true);
					} catch (MySqlException mex) {
						Log(LogLevels.ERROR, $"Error saving user #{c.ID} to db: {mex.Number} - {mex.Message}");
					}

					foreach (var p in Dispatch.State.Players) {
						if (Dispatch.State.GetPlayerIDBySocketID(c.ID) != p.Value.Vnum && player.ConnectionState == ConnectionStates.Connected) {
							Dispatch.SendToUser(p.Value.Vnum, $"`b[`yINFO`b]`0 `c{player.Name}`0 has disconnected!`n");
						}
					}

					Dispatch.State.RemovePlayer(player.Vnum);
				}
			}

			return;
		}
	}
}
