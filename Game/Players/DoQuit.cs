using FW.Core;
using FW.Core.Models;

namespace FW.Game.Players
{
	public class DoQuit : ActionBase
	{
		public DoQuit()
			: base("quit", "quit", "Disconnect from the game")
		{
			return;
		}


		public override void Act(Command Cmd, PlayerPC Player, TickDispatch Dispatch)
		{
			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC && Dispatch.State.GetPlayerIDBySocketID(Cmd.ID) != p.Value.ID) {
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
