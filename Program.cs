using System;

namespace FW
{
	class Program
	{
		static void Main(string[] args)
		{
			/// Events:
			///   - Init: beginning of setup, starts socket server after reading configuration (from CLI or file), starts logging channels
			///   - GameLoop:
			///     - SocketPoll: polls socket server for any new commands (events)
			///     - NewPlayers: any 'accept' commands from SocketServer are handled as new clients
			///     - ProcCommands: any 'recv' commands are parsed here for actions from users
			///     - Tick: go through all wired entities and perform single game tick
			///     - SocketSend: feeds any resulting commands to SocketSErver to be sent to clients
			///   - Shutdown: clean up all memory and socket connections, dump logs if configured, close application

			/// Data Structures:
			///   - ClientCommand: single command to/from a socket
			///   - TickDispatch: state machine carried through processors to deliver commands and other meta information
			///   - Character: basic creature in world
			///   - Player: a PC, child of Character controlled by a socket Client

			Console.WriteLine("Hello World!");
		}
	}
}
