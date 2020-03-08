using System.Collections.Generic;

namespace Stoic.Log
{
	public abstract class AppenderBase : Chain.NodeBase<MessageDispatch, Message, List<Message>>
	{
		protected AppenderBase(string Key, string Version)
			: base(Key, Version)
		{
			return;
		}
	}
}
