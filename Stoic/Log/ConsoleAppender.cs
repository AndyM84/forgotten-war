using System;

namespace Stoic.Log
{
	public class ConsoleAppender : AppenderBase
	{
		protected bool _Colorize;


		public ConsoleAppender(bool Colorize = true)
			: base("ConsoleAppender", "1.0.0")
		{
			this._Colorize = Colorize;

			return;
		}

		public override void Process(ref object Sender, ref MessageDispatch Dispatch)
		{
			if (Dispatch.Messages.Count > 0) {
				foreach (var m in Dispatch.Messages) {
					if (this._Colorize) {
						this.OutputWithColor(m);
					} else {
						Console.WriteLine(m);
					}
				}
			}

			return;
		}

		protected void OutputWithColor(Message Msg)
		{
			switch (Msg.Level) {
				case LogLevels.ALERT:
					Console.ForegroundColor = ConsoleColor.Magenta;
					Console.WriteLine(Msg);
					Console.ResetColor();

					break;

				case LogLevels.CRITICAL:
					Console.ForegroundColor = ConsoleColor.DarkRed;
					Console.WriteLine(Msg);
					Console.ResetColor();

					break;

				case LogLevels.DEBUG:
					Console.ForegroundColor = ConsoleColor.Red;
					Console.WriteLine(Msg);
					Console.ResetColor();

					break;

				case LogLevels.EMERGENCY:
					Console.ForegroundColor = ConsoleColor.Yellow;
					Console.WriteLine(Msg);
					Console.ResetColor();

					break;

				case LogLevels.ERROR:
					Console.ForegroundColor = ConsoleColor.Cyan;
					Console.WriteLine(Msg);
					Console.ResetColor();

					break;

				case LogLevels.INFO:
					Console.ForegroundColor = ConsoleColor.Gray;
					Console.WriteLine(Msg);
					Console.ResetColor();

					break;

				case LogLevels.NOTICE:
					Console.ForegroundColor = ConsoleColor.White;
					Console.WriteLine(Msg);
					Console.ResetColor();

					break;

				case LogLevels.WARNING:
					Console.ForegroundColor = ConsoleColor.White;
					Console.WriteLine(Msg);
					Console.ResetColor();

					break;
			}

			return;
		}
	}
}
