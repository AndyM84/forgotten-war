using System;

namespace FW.Core
{
	public class Config : ICloneable
	{
		public string DbDsn { get; set; }
		public string LogFile { get; set; }
		public int Port { get; set; }


		public object Clone()
		{
			return new Config {
				DbDsn   = this.DbDsn,
				LogFile = this.LogFile,
				Port    = this.Port
			};
		}
	}
}
