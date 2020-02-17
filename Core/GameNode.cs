using System.Collections.Generic;

using Stoic.Chain;
using Stoic.Log;

namespace FW.Core
{
	public abstract class GameNode : NodeBase<TickDispatch, Command, List<Command>>
	{
		protected Logger _Log;


		public GameNode(string Key, string Version, ref Logger Logger)
			: base(Key, Version)
		{
			this._Log = Logger;

			return;
		}


		protected void Log(LogLevels Level, string Message)
		{
			this._Log.Log(Level, Message);

			return;
		}
	}
}
