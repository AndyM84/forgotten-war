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
	}
}
