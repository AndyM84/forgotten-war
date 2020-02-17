using FW.Core;
using Stoic.Log;

namespace FW.Game.Players
{
	public class PlayersNode : GameNode
	{
		public PlayersNode(ref Logger Logger)
			: base("PlayersNode", "1.0", ref Logger)
		{
			this.Log(LogLevels.DEBUG, "Initialized game PLAYERS node");

			return;
		}


		public override void Process(ref object Sender, ref TickDispatch Dispatch)
		{
			//throw new System.NotImplementedException();
		}
	}
}
