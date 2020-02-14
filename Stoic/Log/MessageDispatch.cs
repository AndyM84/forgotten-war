using System;
using System.Collections.Generic;

namespace Stoic.Log
{
	public class MessageDispatch : Chain.DispatchBase<Message, List<Message>>
	{
		protected List<Message> _Messages;

		public List<Message> Messages { get { return this._Messages; } }


		public MessageDispatch()
		{
			this._Messages = new List<Message>();

			return;
		}

		public override void Initialize()
		{
			throw new NotImplementedException();
		}

		public void Initialize(Message Msg)
		{
			if (this.IsValid) {
				return;
			}

			this._Messages.Add(Msg);

			this.MakeValid();

			return;
		}

		public void Initialize(ICollection<Message> Messages)
		{
			if (this.IsValid) {
				return;
			}

			foreach (var m in Messages) {
				this._Messages.Add(m);
			}

			this.MakeValid();

			return;
		}
	}
}
