using System;

namespace FW.Core.Models
{
	public class Mob : Monster
	{
		public Alivenesses Aliveness { get; set; }
		public Location    Location { get; set; }
		public DateTime    Spawned { get; set; }
	}
}
