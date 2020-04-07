using System.Collections.Generic;

namespace FW.Core.Models
{
	public class Monster
	{
		public Dictionary<string, int> Attributes { get; set; }
		public Citizenships            Citizenship { get; set; }
		public Classes                 Class { get; set; }
		public double                  Fatigue { get; set; }
		public string                  Name { get; set; }
		public Races                   Race { get; set; }
		public int                     Vnum { get; set; }


		public Monster(
			int vnum,
			string name,
			Races race,
			Classes cls,
			Citizenships cships,
			Dictionary<string, int> attribs = null,
			double fatigue = 0.0)
		{
			this.Attributes = attribs ?? new Dictionary<string, int>();
			this.Citizenship = cships;
			this.Class = cls;
			this.Fatigue = fatigue;
			this.Name = name;
			this.Race = race;
			this.Vnum = vnum;

			return;
		}
	}
}
