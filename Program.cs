using System;
using System.Collections.Generic;

using FW.Core;
using FW.Game;
using Stoic.Chain;
using Stoic.Log;
using Stoic.Utilities;

namespace FW
{
	class Program
	{
		public static bool ShouldRun = true;


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

			Console.CancelKeyPress += new ConsoleCancelEventHandler(SigIntHandler);

			var ch = new ConsoleHelper(args);
			var logger = new Logger(LogLevels.DEBUG);
			var serv = new SocketServer(10, 5000, ref logger);

			logger.AddAppender(new ConsoleAppender());

			logger.Log(LogLevels.DEBUG, "Initialized game console subsystem");
			logger.Log(LogLevels.DEBUG, "Initialized game logging subsystem");
			logger.Log(LogLevels.DEBUG, "Initialized game socket subsystem");
			logger.Log(LogLevels.INFO, "Finished initializing game subsystems");
			logger.Output();

			var game = new ChainHelper<TickDispatch, Command, List<Command>>();
			game.LinkNode(new Game.Comms.CommsNode(ref logger));
			game.LinkNode(new Game.Players.PlayersNode(ref logger));
			game.LinkNode(new Game.Objects.ObjectsNode(ref logger));
			game.LinkNode(new Game.World.WorldNode(ref logger));
			logger.Output();
			
			while (ShouldRun) {
				var disp = new TickDispatch();
				disp.Initialize(serv.Poll());

				logger.Output();
			}

			logger.Log(LogLevels.INFO, "Shutting down game subsystems");

			serv.Shutdown();
			logger.Output();

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

		public static void SigIntHandler(object sender, ConsoleCancelEventArgs args)
		{
			args.Cancel = true;

			Console.WriteLine();
			Console.WriteLine("Captured SIGINT, shutting down...");

			ShouldRun = false;

			return;
		}
	}
}
