using System;

using FW.Core;
using Stoic.Log;
using Stoic.Utilities;

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

			var ch = new ConsoleHelper(args);
			var logger = new Logger(LogLevels.DEBUG);
			logger.AddAppender(new ConsoleAppender());

			var loopCount = 2500000;
			var serv = new SocketServer(10, 5000, ref logger);
			
			while (--loopCount > 0) {
				serv.Poll();
				logger.Output();
			}

			FinalPause();

			return;
		}

		public static void FinalPause()
		{
			Console.WriteLine();
			Console.Write("Press any key to continue...");
			Console.ReadLine();

			return;
		}
	}
}
