using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game.World
{
	public class DoTransfer : ActionBase
	{
		public DoTransfer(Logger Logger)
			: base("transfer", "transfer <PlayerName> <vnum> <?quiet>", "Allows imms to transfer players to specific rooms", Logger, Mortalities.Immortal)
		{
			return;
		}


		public override void Act(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			var lCmd = Cmd.Body.ToLower().Split(' ');

			if (lCmd.Length < 2) {
				Dispatch.SendToUser(Player.Vnum, "Incorrect usage: transfer <PlayerName> <vnum>`n");

				return;
			}

			Character target = null;
			bool doQuietly = lCmd.Length == 3 && lCmd[2].ToLower() == "quiet";

			foreach (var p in Dispatch.State.Players) {
				if (p.Value.Name.ToLower() == lCmd[0]) {
					target = p.Value;

					break;
				}
			}

			if (target == null) {
				Dispatch.SendToUser(Player.Vnum, "Invalid target for transfer, check spelling`n");

				return;
			}

			if (target.Vnum == Player.Vnum) {
				Dispatch.SendToUser(Player.Vnum, "You can't transfer yourself`n");

				return;
			}

			try {
				var vnum = System.Convert.ToInt32(lCmd[1]);

				var oldRoom = target.Location.Vnum;
				target.Location.Vnum = vnum;
				var output = Utilities.GetRoomOutput(vnum, target, Dispatch);

				if (!string.IsNullOrWhiteSpace(output)) {
					Dispatch.SendToUser(target.Vnum, "You experience a moment of weightlessness as the world distorts around in you a swirl of smoke.`n");
					Dispatch.SendToUser(target.Vnum, output);
					Dispatch.SendToUser(target.Vnum, "You feel normal again as you materialize from a puff of smoke.`n");

					if (!doQuietly) {
						this.DoRoomJoin(target, Dispatch);
						this.DoRoomLeave(oldRoom, target, Dispatch);
					}
				}
			} catch (System.FormatException) {
				this.Log(LogLevels.WARNING, $"Invalid VNUM provided for TRANSFER by user '{Player.Name}': {lCmd[1]}");
				Dispatch.SendToUser(Player.Vnum, "You must enter a valid room number`n");
			} catch (System.Collections.Generic.KeyNotFoundException) {
				this.Log(LogLevels.WARNING, $"Non-existent VNUM provided for TRANSFER by user '{Player.Name}': {lCmd[1]}");
				Dispatch.SendToUser(Player.Vnum, "You must enter an existing room number`n");
			}

			return;
		}

		protected void DoRoomJoin(Character Player, TickDispatch Dispatch)
		{
			string text = $"{Player.Name} enters the room in a puff of smoke.`n";

			foreach (var p in Dispatch.State.Players) {
				// We can put visibility checks and such in here

				if (p.Value.Location.Vnum == Player.Location.Vnum && p.Value.Vnum != Player.Vnum) {
					Dispatch.SendToUser(p.Value.Vnum, text);
				}
			}

			return;
		}

		protected void DoRoomLeave(int Vnum, Character Player, TickDispatch Dispatch)
		{
			string text = $"{Player.Name} leaves the room in a puff of smoke.`n";

			foreach (var p in Dispatch.State.Players) {
				// We can put visibility checks and such in here

				if (p.Value.Location.Vnum == Vnum && p.Value.Vnum != Player.Vnum) {
					Dispatch.SendToUser(p.Value.Vnum, text);
				}
			}

			return;
		}
	}
}
