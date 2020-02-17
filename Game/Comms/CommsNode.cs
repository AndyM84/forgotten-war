using FW.Core;
using Stoic.Log;

namespace FW.Game.Comms
{
	public class CommsNode : GameNode
	{
		public CommsNode(ref Logger Logger)
			: base("CommsNode", "1.0", ref Logger)
		{
			this.Log(LogLevels.DEBUG, "Initialized game COMMS node");

			return;
		}


		public override void Process(ref object Sender, ref TickDispatch Dispatch)
		{
			throw new System.NotImplementedException();
		}
	}
}
