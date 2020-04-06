namespace FW.Core.Models
{
	public class Exit
	{
		public int        Destination { get; set; }
		public Directions Direction { get; set; }
		public bool       Locked { get; set; }
		public bool       Passable { get; set; }
		public int        Source { get; set; }


		public Exit(
			int source,
			int destination,
			Directions direction,
			bool locked,
			bool passable)
		{
			this.Destination = destination;
			this.Direction = direction;
			this.Locked = locked;
			this.Passable = passable;
			this.Source = source;

			return;
		}
	}
}
