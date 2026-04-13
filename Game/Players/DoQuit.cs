using MySql.Data.MySqlClient;
using Stoic.Log;

using FW.Core;
using FW.Core.Models;

namespace FW.Game.Players
{
	public class DoQuit : ActionBase
	{
		public DoQuit(Logger Logger)
			: base("quit", "quit", "Disconnect from the game", Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			foreach (var p in Dispatch.State.Players) {
				if (Dispatch.State.GetPlayerIDBySocketID(Cmd.ID) != p.Value.Vnum) {
					Dispatch.SendToUser(p.Value.Vnum, $"`b[`yINFO`b]`0 `c{Player.Name}`0 has disconnected!\n\n");
				}
			}

			try {
				using var conn = new MySqlConnection(Dispatch.State.Config.DbDsn);
				Player.SaveToDb(conn, true);
			} catch (MySqlException mex) {
				Log(LogLevels.ERROR, $"Error saving user #{Player.Vnum} to db: {mex.Code} - {mex.Message} {mex.StackTrace}");

				var iex = mex.InnerException;

				while (iex != null) {
					Log(LogLevels.ALERT, iex.Message);
					iex = iex.InnerException;
				}
			}

			Dispatch.SendToUser(Player.Vnum, $"`n`nThanks for playing, {Player.Name}!`n`n");
			Dispatch.DisconnectUser(Player.Vnum);

			Dispatch.State.RemovePlayer(Player.Vnum);

			return;
		}
	}
}
