namespace FW.Core.Models
{
	public class Exit
	{
		public int        Destination { get; set; }
		public Directions Direction { get; set; }
		public bool       Locked { get; set; }
		public bool       Passable { get; set; }
		public int        Source { get; set; }
	}
}
