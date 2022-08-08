using System;
using System.Collections.Generic;

using FW.Core.DbModels;

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
		public Poses                   Pose { get; set; }
		public Races                   Race { get; set; }

		#endregion

		#region Connection Properties

		public ConnectionStates        ConnectionState { get; set; }
		public DateTime                Connected { get; set; }
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


		public void HydrateFromPlayerChar(PlayerChar Char)
		{
			this.Class       = (Classes)Char.Class;
			this.Drunk       = Char.Drunk;
			this.Fatigue     = Char.Fatigue;
			this.Luck        = Char.Luck;
			this.Mental      = Char.Mental;
			this.Pose        = (Poses)Char.Pose;
			this.Race        = (Races)Char.Race;
			this.Created     = Char.Created ?? DateTime.UtcNow;
			this.PlayedTime  = TimeSpan.FromMilliseconds(Char.PlayedTime);
			this.Prompt      = Char.Prompt;
			this.Aliveness   = (Alivenesses)Char.Aliveness;
			this.Birthdate   = Char.Birthdate ?? DateTime.UnixEpoch;
			this.Citizenship = (Citizenships)Char.Citizenship;
			this.Location    = new Location(new System.Numerics.Vector3(Char.PosX, Char.PosY, Char.PosZ), Char.PosVnum);
			this.Mortality   = (Mortalities)Char.Mortality;
			this.Name        = Char.Name;
			this.Vnum        = Char.Vnum;

			return;
		}
	}
}
