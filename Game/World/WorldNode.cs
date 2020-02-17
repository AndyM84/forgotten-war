using FW.Core;
using Stoic.Log;

namespace FW.Game.World
{
	public class WorldNode : GameNode
	{
		public WorldNode(ref Logger Logger)
			: base("WorldNode", "1.0", ref Logger)
		{
			this.Log(LogLevels.DEBUG, "Initialized game WORLD node");

			return;
		}


		public override void Process(ref object Sender, ref TickDispatch Dispatch)
		{
			throw new System.NotImplementedException();
		}
	}
}
