using System;
using System.Collections.Generic;

namespace FW.Core.Models
{
	public class Mob : Monster
	{
		public Alivenesses Aliveness { get; set; }
		public Location    Location { get; set; }
		public DateTime    Spawned { get; set; }


		public Mob(
			int vnum,
			string name,
			Location location,
			Races race,
			Classes cls,
			Citizenships cships,
			Alivenesses aliveness,
			Dictionary<string, int> attribs = null,
			double fatigue = 0.0)
			: base(vnum, name, race, cls, cships, attribs, fatigue)
		{
			this.Aliveness = aliveness;
			this.Location = location;
			this.Spawned = DateTime.UtcNow;

			return;
		}
	}
}
