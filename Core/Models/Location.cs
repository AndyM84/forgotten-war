using System.Numerics;

namespace FW.Core.Models
{
	public class Location
	{
		public Vector3 Coordinate { get; set; }
		public double  Vnum { get; set; }


		public Location(Vector3 coordinate, double vnum)
		{
			this.Coordinate = coordinate;
			this.Vnum = vnum;

			return;
		}
	}
}
