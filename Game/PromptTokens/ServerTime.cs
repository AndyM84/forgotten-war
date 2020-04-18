using System;

using FW.Core;
using FW.Core.Models;

namespace FW.Game.PromptTokens
{
	public class ServerTime : PromptTokenBase
	{
		public ServerTime()
			: base("%stime%")
		{
			return;
		}


		public override string GenerateSegment(Character Player, State State)
		{
			return DateTime.UtcNow.ToString("HH:mm:ss");
		}
	}
}
