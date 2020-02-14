using System.Collections.Generic;

namespace Stoic.Log
{
	public class Logger
	{
		protected Chain.ChainHelper<MessageDispatch, Message, List<Message>> _Appenders;
		protected List<Message> _Messages;
		protected LogLevels _MinLevel;


		public Logger(LogLevels MinLevel)
		{
			this._Appenders = new Chain.ChainHelper<MessageDispatch, Message, List<Message>>();
			this._Messages = new List<Message>();
			this._MinLevel = MinLevel;

			return;
		}

		public void AddAppender(AppenderBase Appender)
		{
			this._Appenders.LinkNode(Appender);

			return;
		}

		public void Log(LogLevels Level, string Message)
		{
			this._Messages.Add(new Message(Level, Message));

			return;
		}

		protected bool MeetsMinimumLevel(LogLevels Level)
		{
			return Level >= this._MinLevel;
		}

		public void Output()
		{
			List<Message> messages = new List<Message>();

			foreach (var m in this._Messages) {
				if (this.MeetsMinimumLevel(m.Level)) {
					messages.Add(m);
				}
			}

			if (messages.Count < 1) {
				return;
			}

			MessageDispatch disp = new MessageDispatch();
			disp.Initialize(messages);

			this._Messages = new List<Message>();

			this._Appenders.Traverse(ref disp, this);

			return;
		}
	}
}
