using System.Text;

using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game
{
	public class DoHelp : ActionBase
	{
		public DoHelp(Logger Logger)
			: base("help", "help <?keyword>", "Offers help on various topics", Logger)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			var lCmd = Cmd.Body.ToLower();
			StringBuilder sb = new StringBuilder();

			if (lCmd == "help") {
				sb.Append("Help is available on the following topics:`n`n");
				sb.Append("   color`n");
				sb.Append("   commands`n");
				sb.Append("   gameinfo`n`n");
				sb.Append("Enter a topic with the following syntax: help <topic>`n");

				Dispatch.SendToUser(Player.Vnum, sb.ToString());

				return;
			}

			if (lCmd == "color" || lCmd == "colors") {
				sb.Append("`YC`Bo`Rl`Co`Mr`w is accessed with the ` character.`n`n");
				sb.Append("Just type ` followed by one of the following:`n`n");
				sb.Append("   k - Black   K - Light Black`n");
				sb.Append("   y - Yellow  Y - Light Yellow`n");
				sb.Append("   g - Green   G - Light Green`n");
				sb.Append("   b - Blue    B - Light Blue`n");
				sb.Append("   r - Red     R - Light Red`n");
				sb.Append("   c - Cyan    C - Light Cyan`n");
				sb.Append("   m - Magenta M - Light Magenta`n");
				sb.Append("   w - Normal  W - White`n");
			} else if (lCmd == "commands") {
				sb.Append("To see a list of your available commands, send the following:`n`n");
				sb.Append("   commands`n");
			} else if (lCmd == "gameinfo") {
				sb.Append("This is Forgotten War!`n`n");
				sb.Append("   Version:  " + Dispatch.State.Version + "`n");
				sb.Append("   Branch:   " + Dispatch.State.Branch + "`n");
				sb.Append("   Commit:   " + Dispatch.State.Commit + "`n");
				sb.Append("   Creators: Kyssandra`n");
				sb.Append("             Neryndil`n");
				sb.Append("             Xitan`n");
			} else {
				sb.Append("There were no help topics that matched your query.`n");
			}

			Dispatch.SendToUser(Player.Vnum, sb.ToString());

			return;
		}
	}
}
