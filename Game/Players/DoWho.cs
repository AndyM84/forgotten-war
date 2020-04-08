using System.Text;

using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.Players
{
	public class DoWho : ActionBase
	{
		public DoWho(Logger Logger)
			: base("who", "who", "Display a list of online visible players", Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			StringBuilder sb = new StringBuilder("=== Online Players ===`n");

			foreach (var p in Dispatch.State.Players) {
				if (p.Value.ConnectionState == ConnectionStates.Connected) {
					sb.Append("`b---`0 ");
					sb.Append(p.Value.Name);

					if (p.Value.Vnum == Player.Vnum) {
						sb.Append(" `w(YOU)`0");
					}

					sb.Append("`n");
				}
			}

			sb.Append("======================`n`n");
			Dispatch.SendToUser(Player.Vnum, sb.ToString());

			return;
		}
	}
}
