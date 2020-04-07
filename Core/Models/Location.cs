using System.Numerics;

namespace FW.Core.Models
{
	public class Location
	{
		public Vector3 Coordinate { get; set; }
		public int  Vnum { get; set; }


		public Location(Vector3 coordinate, int vnum)
		{
			this.Coordinate = coordinate;
			this.Vnum = vnum;

			return;
		}
	}
}
