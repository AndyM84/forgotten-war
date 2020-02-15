using System;
using System.Collections.Generic;
using System.IO;

namespace Stoic.Utilities
{
	public struct ParsedArgument
	{
		public string Key { get; set; }
		public string LoweredKey { get; set; }
		public string LoweredValue { get; set; }
		public string Original { get; set; }
		public string Value { get; set; }
	}

	public class ConsoleHelper
	{
		protected Dictionary<string, ParsedArgument> _Arguments;
		protected string[] _OriginalArguments;
		protected bool _Windows;

		public Dictionary<string, ParsedArgument> Arguments { get { return this._Arguments; } }
		public bool IsWindows { get { return this._Windows; } }
		public List<string> OriginalArguments { get { return new List<string>(this._OriginalArguments); } }


		public ConsoleHelper()
			: this(new string[0])
		{
			return;
		}

		public ConsoleHelper(string[] Arguments)
		{
			this._Arguments = this.ParseArguments(Arguments);
			this._OriginalArguments = Arguments;
			this._Windows = Path.DirectorySeparatorChar == '\\';

			return;
		}


		public bool CompareArg(string Key, string Value, bool CaseInsensitive = false)
		{
			var lKey = Key.ToLower();
			var lVal = Value.ToLower();

			if (!this._Arguments.ContainsKey(lKey)) {
				return false;
			}

			if (CaseInsensitive) {
				return this._Arguments[lKey].LoweredValue == lVal;
			}

			return this._Arguments[lKey].Value == Value;
		}

		public string Get(int Characters = 1)
		{
			var ret = string.Empty;

			do {
				var input = Console.ReadLine();

				if (ret.Length + input.Length > Characters) {
					var limit = Characters - ret.Length;
					ret += input.Substring(0, limit);
				} else {
					ret += input;
				}
			} while (ret.Length < Characters);

			return ret.Trim();
		}

		public string GetLine()
		{
			return Console.ReadLine();
		}

		public string GetParameter(string Short, string Long, string Default = null, bool CaseInsensitive = false)
		{
			if (Default == null) {
				Default = string.Empty;
			}

			var lKeyS = Short.ToLower();
			var lKeyL = Long.ToLower();

			if (this._Arguments.ContainsKey(lKeyS)) {
				return (CaseInsensitive) ? this._Arguments[lKeyS].LoweredValue : this._Arguments[lKeyS].Value;
			}

			if (this._Arguments.ContainsKey(lKeyL)) {
				return (CaseInsensitive) ? this._Arguments[lKeyL].LoweredValue : this._Arguments[lKeyL].Value;
			}

			return Default;
		}

		public bool HasArg(string Key)
		{
			return this._Arguments.ContainsKey(Key.ToLower());
		}

		public bool HasShortLongArg(string Short, string Long)
		{
			return this._Arguments.ContainsKey(Short.ToLower()) || this._Arguments.ContainsKey(Long.ToLower());
		}

		protected Dictionary<string, ParsedArgument> ParseArguments(string[] Arguments)
		{
			var ret = new Dictionary<string, ParsedArgument>();

			if (Arguments == null || Arguments.Length < 1) {
				return ret;
			}

			for (int i = 0; i < Arguments.Length; i++) {
				var tmp = new ParsedArgument {
					Key = string.Empty,
					Original = Arguments[i],
					Value = string.Empty
				};

				if (Arguments[i].StartsWith('-') && Arguments[i].Length > 1) {
					string arg = Arguments[i].Substring((Arguments[i].StartsWith("--")) ? 2 : 1);
					int eqIndex = arg.IndexOf('=');
					int dsIndex = arg.IndexOf('-');

					if (eqIndex > 1 && eqIndex != arg.Length) {
						tmp.Key = arg.Substring(0, eqIndex);
						tmp.Value = arg.Substring(eqIndex + 1);
					} else if (dsIndex > 1 && dsIndex != arg.Length) {
						tmp.Key = arg.Substring(0, dsIndex);
						tmp.Value = arg.Substring(dsIndex + 1);
					} else if ((i + 1) < Arguments.Length && (!Arguments[i + 1].StartsWith('-') && !Arguments[i + 1].StartsWith('='))) {
						tmp.Key = arg;
						tmp.Original += " " + Arguments[i + 1];
						tmp.Value = Arguments[++i];
					} else {
						tmp.Key = arg;
						tmp.Value = "true";
					}
				} else {
					if (Arguments[i].IndexOf('=') > 1) {
						var parts = Arguments[i].Split('=');
						tmp.Key = parts[0];
						tmp.Value = parts[1];
					} else {
						tmp.Key = Arguments[i];
						tmp.Value = "true";
					}
				}

				if (string.IsNullOrWhiteSpace(tmp.Key) && string.IsNullOrWhiteSpace(tmp.Value)) {
					continue;
				}

				tmp.LoweredKey = tmp.Key.ToLower();
				tmp.LoweredValue = tmp.Value.ToLower();

				ret.Add(tmp.LoweredKey, tmp);
			}

			return ret;
		}

		public void Put(string Buffer)
		{
			Console.Write(Buffer);

			return;
		}

		public void PutLine(string Buffer = null)
		{
			if (Buffer != null) {
				Console.WriteLine(Buffer);
			} else {
				Console.WriteLine();
			}

			return;
		}

		public ReturnHelper<string> QueryInput(string Query, string DefaultValue, string ErrorMessage, int MaxTries = 5, Func<string, bool> Validation = null, Func<string, string> Sanitation = null)
		{
			var ret = new ReturnHelper<string>();
			var prompt = Query;

			if (!string.IsNullOrWhiteSpace(DefaultValue)) {
				prompt += " [" + DefaultValue + "]";
			}

			prompt += ": ";

			if (Validation == null) {
				Validation = v => !string.IsNullOrWhiteSpace(v.Trim());
			}

			if (Sanitation == null) {
				Sanitation = v => v.Trim();
			}

			int attempts = 0;

			do {
				this.Put(prompt);
				var resp = this.GetLine();

				if (string.IsNullOrWhiteSpace(resp) && !string.IsNullOrWhiteSpace(DefaultValue)) {
					ret.MakeGood();
					ret.AddResult(Sanitation(resp));

					break;
				}

				var valid = Validation(resp);

				if (valid) {
					ret.MakeGood();
					ret.AddResult(Sanitation(resp));

					break;
				}

				this.PutLine("** " + ErrorMessage);
				attempts++;
			} while (attempts < MaxTries);

			return ret;
		}
	}
}
