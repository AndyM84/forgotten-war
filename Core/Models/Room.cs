using System.Collections.Generic;
using System.Numerics;

namespace FW.Core.Models
{
	public class Room
	{
		public Biomes                       Biome { get; set; }
		public Vector3                      Coordinate { get; set; }
		public string                       Description { get; set; }
		public Dictionary<Directions, Exit> Exits { get; set; }
		public int                          Mana { get; set; }
		public bool                         NoCombat { get; set; }
		public bool                         NoMagic { get; set; }
		public List<Object>                 Objects { get; set; }
		public Terrains                     Terrain { get; set; }
		public int                          Vnum { get; set; }
	}
}
