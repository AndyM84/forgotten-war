using System;

namespace FW.Core.DbModels
{
	public class PlayerChar
	{
		#region Attribute Properties

		public int                     Class { get; set; }
		public double                  Drunk { get; set; }
		public double                  Fatigue { get; set; }
		public double                  Luck { get; set; }
		public double                  Mental { get; set; }
		public int                     Pose { get; set; }
		public int                     Race { get; set; }

		#endregion

		#region Connection Properties

		public DateTime?               Created { get; set; }
		public double                  PlayedTime { get; set; }
		public string                  Prompt { get; set; }

		#endregion

		#region Ident Properties

		public int                     Aliveness { get; set; }
		public DateTime?               Birthdate { get; set; }
		public int                     Citizenship { get; set; }
		public int                     Mortality { get; set; }
		public string                  Name { get; set; }
		public string                  NameLowered { get; set; }
		public int                     Vnum { get; set; }

		#endregion

		#region Db Only Properties

		public float                   PosX { get; set; }
		public float                   PosY { get; set; }
		public float                   PosZ { get; set; }
		public int                     PosVnum { get; set; }
		public string                  Password { get; set; }
		public bool                    PasswordNeedsChanged { get; set; }

		#endregion
	}
}
