using System;
using System.Collections.Generic;
using System.Text;

namespace FW.Game.Players
{
	public enum ConnectionStates
	{
		NamePrompt,
		PasswordPrompt,
		ColorPrompt,
		Connected
	}

	public class PlayerPC : Player
	{
		public ConnectionStates ConnectionState { get; set; }
		public string Prompt { get; set; }
		public bool ShowColor { get; set; }
		public int SocketID { get; set; }
	}
}
