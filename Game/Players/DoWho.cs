using System.Text;

using FW.Core;
using FW.Core.Models;

namespace FW.Game.Players
{
	public class DoWho : ActionBase
	{
		public DoWho()
			: base("who", "who", "Display a list of online visible players")
		{
			return;
		}


		public override void Act(Command Cmd, PlayerPC Player, TickDispatch Dispatch)
		{
			StringBuilder sb = new StringBuilder("`n=== Online Players ===`n");

			foreach (var p in Dispatch.State.Players) {
				if (p.Value is PlayerPC && ((PlayerPC)p.Value).ConnectionState == ConnectionStates.Connected) {
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
	}
}
