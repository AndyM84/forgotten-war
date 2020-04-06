using System;
using System.Collections.Generic;

namespace FW.Core.Models
{
	public class Character
	{
		#region Attribute Properties

		public Dictionary<string, int> Attributes { get; set; }
		public Classes                 Class { get; set; }
		public double                  Drunk { get; set; }
		public double                  Fatigue { get; set; }
		public double                  Luck { get; set; }
		public double                  Mental { get; set; }
		public Races                   Race { get; set; }

		#endregion

		#region Connection Properties

		public ConnectionStates        ConnectionState { get; set; }
		public DateTime                Created { get; set; }
		public TimeSpan                PlayedTime { get; set; }
		public string                  Prompt { get; set; }
		public bool                    ShowColor { get; set; }
		public int                     SocketID { get; set; }

		#endregion

		#region Ident Properties

		public Alivenesses             Aliveness { get; set; }
		public DateTime                Birthdate { get; set; }
		public Citizenships            Citizenship { get; set; }
		public Location                Location { get; set; }
		public Mortalities             Mortality { get; set; }
		public string                  Name { get; set; }
		public int                     Vnum { get; set; }

		#endregion


		public Character()
		{
			this.Attributes = new Dictionary<string, int>();

			return;
		}
	}
}
