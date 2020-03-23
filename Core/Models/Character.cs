using System;
using System.Collections.Generic;

namespace FW.Core.Models
{
	public class Character
	{
		public int Vnum { get; set; }
		public string Name { get; set; }
		public Races Race { get; set; }
		public Classes Class { get; set; }
		public Dictionary<string, int> Attributes { get; set; }
		public double Luck { get; set; }
		public double Drunk { get; set; }
		public double Mental { get; set; }
		public Mortalities Mortality { get; set; }
		public DateTime Created { get; set; }
		public TimeSpan PlayedTime { get; set; }
		public DateTime Birthdate { get; set; }
		public Alivenesses Aliveness { get; set; }
		public Citizenships Citizenship { get; set; }
		public double Fatigue { get; set; }
	}
}
