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
			foreach (var c in Dispatch.Commands) {
				if (c.Type == CommandTypes.CONNECTED) {
					Dispatch.SendToUser(c.ID, "Welcome to Forgotten War!\n\n");
				} else if (c.Type == CommandTypes.RECEIVED) {
					Dispatch.SendToUser(c.ID, "Command received: " + c.Prefix + "\n  \"" + c.Body + "\"\n\n");
				}
			}

			return;
		}
	}
}
