using System.Collections.Generic;

using FW.Core.Models;
using Stoic.Log;

namespace FW.Core
{
	public abstract class ActionBase
	{
		protected Dictionary<string, Action> _Actions;
		protected Logger _Logger;


		public Dictionary<string, Action> Actions { get { return this._Actions; } }


		protected ActionBase(string Command, string Syntax, string Description, Logger Logger, Mortalities MinMortality = Mortalities.Mortal)
			: this(new Action[1] { new Action(Command, Description, Syntax, MinMortality) }, Logger)
		{
			return;
		}

		protected ActionBase(Action[] Actions, Logger Logger)
		{
			this._Actions = new Dictionary<string, Action>();
			this._Logger = Logger;

			if (Actions.Length > 0) {
				foreach (var a in Actions) {
					this._Actions.Add(a.Command.ToLower(), a);
				}
			}

			return;
		}


		public abstract void Act(Command Cmd, Models.Character Player, TickDispatch Dispatch);

		protected void Log(LogLevels Level, string Message)
		{
			this._Logger.Log(Level, Message);

			return;
		}
	}
}
