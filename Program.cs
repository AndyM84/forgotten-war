using System;
using System.Collections.Generic;

using FW.Core;
using FW.Core.Models;
using FW.Game;
using Stoic.Chain;
using Stoic.Log;
using Stoic.Utilities;

namespace FW
{
	class Program
	{
		public static bool ClosedBySignal = false;
		public static bool ShouldRun = true;


		static void Main(string[] args)
		{
			Console.CancelKeyPress += new ConsoleCancelEventHandler(SigIntHandler);

			var ch = new ConsoleHelper(args);
			var logger = new Logger(LogLevels.DEBUG);
			var serv = new SocketServer(10, Convert.ToInt32(ch.GetParameter("p", "port", "6055")), ref logger);
			var state = new State();

			state.CurrentUserID = 0;
			state.Players = new Dictionary<int, Character>();
			state.PlayerSocketLookup = new Dictionary<int, int>();
			logger.AddAppender(new ConsoleAppender());
			logger.AddAppender(new FileAppender(ch.GetParameter("lf", "log-file", "fw-" + DateTime.Now.ToString("yyyy-MM-dd") + ".log"), FileAppenderOutputTypes.PLAIN));

			logger.Log(LogLevels.DEBUG, "Initialized game console subsystem");
			logger.Log(LogLevels.DEBUG, "Initialized game logging subsystem");
			logger.Log(LogLevels.DEBUG, "Initialized game socket subsystem");
			logger.Log(LogLevels.DEBUG, "Initialized game state subsystem");
			logger.Log(LogLevels.INFO, "Finished initializing game subsystems");
			logger.Output();

			var game = new ChainHelper<TickDispatch, Command, List<Command>>();
			game.LinkNode(new Game.ActionNode(ref logger));
			game.LinkNode(new Game.Objects.ObjectsNode(ref logger));
			game.LinkNode(new Game.World.WorldNode(ref logger));
			game.LinkNode(new Game.Players.PlayersNode(ref logger));
			logger.Output();

			int errCount = 0;
			
			while (ShouldRun) {
				var disp = new TickDispatch();
				bool gotSockErr = false;

				try {
					disp.Initialize(state, serv.Poll());
				} catch (Exception ex) {
					logger.Log(LogLevels.ERROR, ex.Message);
					logger.Log(LogLevels.DEBUG, ex.StackTrace);

					var cmds = new List<Command>();

					foreach (var s in serv.Sockets) {
						if (!s.Value.Socket.Connected) {
							cmds.Add(new Command {
								Contents = "DISCONNECT",
								ID = s.Key,
								Type = CommandTypes.DISCONNECTED
							});
						}
					}

					gotSockErr = true;
					disp.Initialize(state, cmds);
				}

				if (gotSockErr) {
					if (++errCount > 25) {
						ShouldRun = false;
					}
				} else {
					errCount = 0;
				}

				game.Traverse(ref disp);

				if (disp.Results.Count > 0) {
					foreach (var c in disp.Results) {
						if (c.Type == CommandTypes.SEND) {
							serv.Send(c.ID, c.Contents);
						} else if (c.Type == CommandTypes.DISCONNECTED) {
							serv.Close(c.ID);
						}
					}
				}

				logger.Output();
			}

			logger.Log(LogLevels.INFO, "Shutting down game subsystems");

			serv.Shutdown();
			logger.Output();

			if (!ClosedBySignal) {
				FinalPause();
			}

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
			ClosedBySignal = true;

			return;
		}
	}
}
