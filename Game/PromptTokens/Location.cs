using FW.Core;
using FW.Core.Models;

namespace FW.Game.PromptTokens
{
	public class Location : PromptTokenBase
	{
		public Location()
			: base("%loc%", Mortalities.Immortal)
		{
			return;
		}


		public override string GenerateSegment(Character Player, State State)
		{
			string segment = $"VNUM: {Player.Location.Vnum}";

			if (Player.Mortality == Mortalities.Admin) {
				segment = $"LOC: {Player.Location.Coordinate.X},{Player.Location.Coordinate.Y},{Player.Location.Coordinate.Z} / " + segment;
			}

			return segment;
		}
	}
}
