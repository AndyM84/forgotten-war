﻿namespace FW.Core.Models
{
	public class PlayerPC : Player
	{
		public ConnectionStates ConnectionState { get; set; }
		public string Prompt { get; set; }
		public bool ShowColor { get; set; }
		public int SocketID { get; set; }
	}
}
