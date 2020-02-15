using System;
using System.Collections.Generic;

namespace Stoic.Log
{
	public class Message
	{
		protected const string TimestampFormat = "yyyy-MM-dd HH:mm:ss.fff";

		protected string _Contents;
		protected LogLevels _Level;
		protected DateTime _TimeStamp;
		private static Dictionary<LogLevels, string> _LogLevels = new Dictionary<LogLevels, string>{
			{ LogLevels.DEBUG,     "DEBUG" },
			{ LogLevels.INFO,      "INFO" },
			{ LogLevels.NOTICE,    "NOTICE" },
			{ LogLevels.WARNING,   "WARNING" },
			{ LogLevels.ERROR,     "ERROR" },
			{ LogLevels.CRITICAL,  "CRITICAL" },
			{ LogLevels.ALERT,     "ALERT" },
			{ LogLevels.EMERGENCY, "EMERGENCY" }
		};

		public string Contents { get { return this._Contents; } }
		public LogLevels Level { get { return this._Level; } }
		public DateTime TimeStamp { get { return this._TimeStamp; } }


		public Message(LogLevels Level, string Contents)
		{
			this._Contents = Contents;
			this._Level = Level;
			this._TimeStamp = DateTime.UtcNow;

			return;
		}


		public Dictionary<string, string> ToDictionary()
		{
			return new Dictionary<string, string> {
				{ "level", _LogLevels[this._Level] },
				{ "message", this._Contents },
				{ "timestamp", this._TimeStamp.ToString(TimestampFormat) }
			};
		}

		public string ToJson()
		{
			return string.Format(
				"{{ \"level\": \"{0}\", \"message\": \"{1}\", \"timestamp\": \"{2}\" }}",
				_LogLevels[this._Level],
				this._Contents,
				this._TimeStamp.ToString(TimestampFormat)
			);
		}

		public override string ToString()
		{
			return string.Format(
				"{0} {1,-9} {2}",
				this._TimeStamp.ToString(TimestampFormat),
				_LogLevels[this._Level],
				this._Contents
			);
		}
	}
}
