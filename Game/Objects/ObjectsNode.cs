using FW.Core;
using Stoic.Log;

namespace FW.Game.Objects
{
	public class ObjectsNode : GameNode
	{
		public ObjectsNode(ref Logger Logger)
			: base("ObjectsNode", "1.0", ref Logger)
		{
			this.Log(LogLevels.DEBUG, "Initialized game OBJECTS node");

			return;
		}


		public override void Process(ref object Sender, ref TickDispatch Dispatch)
		{
			//throw new System.NotImplementedException();
		}
	}
}
