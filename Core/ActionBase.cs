namespace FW.Core
{
	public abstract class ActionBase
	{
		protected string _Command;
		protected string _Description;
		protected string _Syntax;
		// TODO: Need to add the ability to limit these based on our 'mortality' setting (admin/imm commands, etc)  - AndyM84


		public string Command { get { return this._Command; } }
		public string Description { get { return this._Description; } }
		public string Syntax { get { return this._Syntax; } }


		protected ActionBase(string Command, string Syntax, string Description)
		{
			this._Command = Command;
			this._Description = Description;
			this._Syntax = Syntax;

			return;
		}


		public abstract void Act(Command Cmd, Models.Character Player, TickDispatch Dispatch);
	}
}
