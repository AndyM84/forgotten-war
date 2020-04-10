namespace FW.Core.Models
{
	public class Action
	{
		public string Command { get; set; }
		public string Description { get; set; }
		public Mortalities MinMortality { get; set; }
		public string Syntax { get; set; }
		public bool Visible { get; set; }


		public Action(string Command, string Description, string Syntax, Mortalities MinMortality = Mortalities.Mortal, bool Visible = false)
		{
			this.Command = Command;
			this.Description = Description;
			this.MinMortality = MinMortality;
			this.Syntax = Syntax;
			this.Visible = Visible;

			return;
		}
	}
}
