using FW.Core;
using FW.Core.Models;
using Stoic.Log;

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

			Dispatch.SendToUser(Player.Vnum, $"`n`nThanks for playing, {Player.Name}!`n`n");
			Dispatch.DisconnectUser(Player.Vnum);

			Dispatch.State.RemovePlayer(Player.Vnum);

			return;
		}
	}
}
