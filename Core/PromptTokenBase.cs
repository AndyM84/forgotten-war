using System.Collections.Generic;

using FW.Core.Models;

namespace FW.Core
{
	public abstract class PromptTokenBase
	{
		public List<PromptToken> PromptTokens { get; set; }


		protected PromptTokenBase(string Token, Mortalities MinMortality = Mortalities.Mortal)
			: this(new PromptToken[1] { new PromptToken { Token = Token, MinMortality = MinMortality } })
		{
			return;
		}

		protected PromptTokenBase(PromptToken[] Tokens)
		{
			this.PromptTokens = new List<PromptToken>();

			foreach (var t in Tokens) {
				this.PromptTokens.Add(t);
			}

			return;
		}


		public abstract string GenerateSegment(Character Player, State State);
	}
}
