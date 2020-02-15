using System;
using System.Collections.Generic;
using System.IO;

namespace Stoic.Utilities
{
	public struct ParsedArgument
	{
		public string Key { get; set; }
		public string LoweredKey { get; set; }
		public string Original { get; set; }
		public string Value { get; set; }
	}

	public class ConsoleHelper
	{
		protected List<ParsedArgument> _Arguments;
		protected bool _Windows;
		protected string[] _OriginalArguments;

		public List<ParsedArgument> Arguments { get { return this._Arguments; } }
		public bool IsWindows { get { return this._Windows; } }
		public List<string> OriginalArguments { get { return new List<string>(this._OriginalArguments); } }



	}
}
