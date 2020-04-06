using System;
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


		public Room(
			int vnum,
			Vector3 coordinate,
			Exit[] exits,
			bool noCombat,
			bool noMagic,
			Biomes biome,
			Terrains terrain,
			string description,
			Object[] objects = null,
			int mana = 0)
		{
			this.Biome = biome;
			this.Coordinate = coordinate;
			this.Description = description;
			this.Exits = new Dictionary<Directions, Exit>();
			this.Mana = mana;
			this.NoCombat = noCombat;
			this.NoMagic = noMagic;
			this.Objects = new List<Object>(objects ?? Array.Empty<Object>());
			this.Terrain = terrain;
			this.Vnum = vnum;

			foreach (var e in exits) {
				this.Exits[e.Direction] = e;
			}

			return;
		}
	}
}
