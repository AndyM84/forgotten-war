using System;
using System.Collections.Generic;
using System.Text;

using FW.Core;
using FW.Core.Models;
using Stoic.Log;

namespace FW.Game
{
	public class ActionNode : GameNode
	{
		protected List<string> _ActionList;
		protected Dictionary<string, ActionBase> _Actions;
		protected int _ActionWidth;


		public ActionNode(ref Logger Logger)
			: base("ActionNode", "1.0", ref Logger)
		{
			this.Log(LogLevels.DEBUG, "Initializing game ACTION node");

			this._ActionWidth = 8;
			Type baseActType = typeof(ActionBase);
			this._ActionList = new List<string>();
			this._Actions = new Dictionary<string, ActionBase>();

			foreach (var asm in AppDomain.CurrentDomain.GetAssemblies()) {
				foreach (var t in asm.GetTypes()) {
					if (baseActType.IsAssignableFrom(t) && !t.IsAbstract) {
						var tmp = Activator.CreateInstance(t, Logger);
						
						if (tmp != null) {
							var cmd = ((ActionBase)tmp).Command.ToLower();
							this._Actions.Add(cmd, (ActionBase)tmp);

							if (cmd.Length > this._ActionWidth) {
								this._ActionWidth = cmd.Length;
							}

							this.Log(LogLevels.DEBUG, " - Loaded the '" + cmd + "' action");
						}
					}
				}
			}

			this._ActionList.Add(string.Format("{0,-" + this._ActionWidth + "} {1}", "commands", "Display the list of all available commands"));

			foreach (var action in this._Actions) {
				this._ActionList.Add(string.Format("{0,-" + this._ActionWidth + "} {1}", action.Value.Command, action.Value.Description));
			}

			this.Log(LogLevels.DEBUG, "Initialized game ACTION node");

			return;
		}


		public override void Process(ref object Sender, ref TickDispatch Dispatch)
		{
			foreach (var c in Dispatch.Commands) {
				if (c.Type != CommandTypes.RECEIVED) {
					continue;
				}

				var cmd = c.Prefix.ToLower();
				var player = Dispatch.State.GetPlayerBySocketID(c.ID);

				if (player == null) {
					continue;
				}

				if (cmd == "commands") {
					this.DoCommands(c, player, Dispatch);
				}

				if (this._Actions.ContainsKey(cmd)) {
					this._Actions[cmd].Act(c, player, Dispatch);
				}
			}

			return;
		}

		public void DoCommands(Command Cmd, Character Player, TickDispatch Dispatch)
		{
			StringBuilder sb = new StringBuilder("`nAvailable commands:`n`n");

			foreach (var a in this._ActionList) {
				sb.Append(a);
				sb.Append("`n");
			}

			sb.Append("`n`n");

			Dispatch.SendToUser(Player.Vnum, sb.ToString());

			return;
		}
	}
}
