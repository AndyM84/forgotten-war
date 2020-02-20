using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FW.Game.Players
{
	public class Player
	{
		public int ID { get; set; }
		public Vector3 CoordLocation { get; set; }
		public string Name { get; set; }
		public double VnumLocation { get; set; }
	}
}
