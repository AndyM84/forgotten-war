using FW.Core.Models;
using Stoic.Log;

namespace FW.Core
{
	public abstract class ActionBase
	{
		protected string _Command;
		protected string _Description;
		protected Logger _Logger;
		protected Mortalities _MinMortality;
		protected string _Syntax;


		public string Command { get { return this._Command; } }
		public string Description { get { return this._Description; } }
		public Mortalities MinMortality { get { return this._MinMortality; } }
		public string Syntax { get { return this._Syntax; } }


		protected ActionBase(string Command, string Syntax, string Description, Logger Logger, Mortalities MinMortality = Mortalities.Mortal)
		{
			this._Command = Command;
			this._Description = Description;
			this._Logger = Logger;
			this._MinMortality = MinMortality;
			this._Syntax = Syntax;

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
