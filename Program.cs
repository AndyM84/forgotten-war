using System;
using System.Collections.Generic;
using System.IO;
using System.Numerics;

using FW.Core;
using FW.Core.Models;
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

			state.Branch = (File.Exists("branch.txt")) ? File.ReadAllText("branch.txt").Trim() : "master";
			state.Commit = (File.Exists("commit.txt")) ? File.ReadAllText("commit.txt").Trim() : "000000";
			state.Version = (File.Exists("version.txt")) ? File.ReadAllText("version.txt").Trim() : "0.0.0.0";

			#region Rooms (Temp)

			//////////////////////////////
			// TEMP ROOM INITIALIZATION //
			//////////////////////////////

			state.AddRoom(new Room(
				1,
				"Common Room",
				new Vector3(0.0f, 0.0f, 0.0f),
				new Exit[2] {
					new Exit(1, 2, Directions.East, false, true),
					new Exit(1, 3, Directions.Up, false, true)
				},
				true,
				true,
				Biomes.Interior,
				Terrains.Interior,
				@"The atmosphere of this common room is jovial and bustling. A duo of minstrels are belting songs of bravery near a stone fireplace, with a half circle of benches splayed out in front. A wooden bar along the western wall features a half dozen tapped kegs and shelving full of steins. Around the room are tables full of adventures sharing stories, dicing, eating, drinking, and playing cards.",
				null,
				20));
			state.AddRoom(new Room(
				2,
				"Private Booth",
				new Vector3(0.0f, 0.0f, 0.0f),
				new Exit[1] {
					new Exit(2, 1, Directions.West, false, true)
				},
				true,
				true,
				Biomes.Interior,
				Terrains.Interior,
				@"Tucked in the back corner of the tavern, this secluded booth provides ample opportunity for private conversation between those sitting around the table. Several pieces of empty glassware are stacked in the center of the table, showing that this booth is used regularly and not often approached by the waitresses",
				null,
				20));
			state.AddRoom(new Room(
				3,
				"Upstairs Hallway",
				new Vector3(0.0f, 0.0f, 0.0f),
				new Exit[4] {
					new Exit(3, 1, Directions.Down, false, true),
					new Exit(3, 4, Directions.East, false, true),
					new Exit(3, 5, Directions.North, false, true),
					new Exit(3, 6, Directions.West, false, true)
				},
				true,
				true,
				Biomes.Interior,
				Terrains.Interior,
				@"A wide hallway runs the length of the building, with sturdy wooden doors leading to individual rooms in each direction. Small candles are nestled into wall sconces lining the walls, providing ample light in this area no matter the time of day. Small placards have been hand engraved and nailed to each door.",
				null,
				20));
			state.AddRoom(new Room(
				4,
				"Xitan's Roomo",
				new Vector3(0.0f, 0.0f, 0.0f),
				new Exit[1] {
					new Exit(4, 3, Directions.West, false, true)
				},
				true,
				true,
				Biomes.Interior,
				Terrains.Interior,
				//@"There isn't much left of this room's floor as far as you can see, it's mostly covered with all manner of paraphernalia.  Among the many things strewn about are wood working tools, books, weapons, strange armor, and a half-finished painting in a back corner.  One of the notebooks appears to say ""Xitan's Bridge Designs.""  You notice there are obvious paths through the clutter you can walk through, one of which leads to a pillow and rolled up blanket on the floor.",
				"Everything slows down, as you turn into a turtle upon entering the room..",
				null,
				20));
			state.AddRoom(new Room(
				5,
				"Kyssandra's Room",
				new Vector3(0.0f, 0.0f, 0.0f),
				new Exit[1] {
					new Exit(5, 3, Directions.South, false, true)
				},
				true,
				true,
				Biomes.Interior,
				Terrains.Interior,
				@"This room is devoid of a majority of the furniture you'd expect in a bedroom, except for the bed itself. Hundreds of ivory wax candles are scattered around the mostly empty floor, making it nearly impossible to walk anywhere but to and from the bed itself. The canopied bed is draped in a luxurious ivory linen with edgings of gold that glint as the glow of the candles dance across the metallic fibers. Carved into the soft wood of one of the bedposts is the name 'Kyssandra' flanked by two small stars.",
				null,
				20));
			state.AddRoom(new Room(
				6,
				"Neryndil's Room",
				new Vector3(0.0f, 0.0f, 0.0f),
				new Exit[1] {
					new Exit(6, 3, Directions.East, false, true)
				},
				true,
				true,
				Biomes.Interior,
				Terrains.Interior,
				@"",
				null,
				20));

			//////////////////////////////
			// TEMP ROOM INITIALIZATION //
			//////////////////////////////

			#endregion

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
