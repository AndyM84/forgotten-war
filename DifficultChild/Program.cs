using System;

using Stoic.Log;
using Stoic.Utilities;

namespace DifficultChild
{
	class Program
	{
		public static bool ShouldRun = true;


		static void Main(string[] args)
		{
			Console.CancelKeyPress += new ConsoleCancelEventHandler(SigIntHandler);

			var ch = new ConsoleHelper(args);
			var logger = new Logger(LogLevels.DEBUG);
			var settings = new Settings {
				Host = ch.GetParameter("h", "host", "nebula.zibings.net"),
				Port = Convert.ToInt32(ch.GetParameter("p", "port", "6055"))
			};

			logger.AddAppender(new ConsoleAppender());
			logger.Log(LogLevels.INFO, "Let's see if any tantrums are thrown!\n");
			logger.Output();

			Type ti = typeof(Tantrums.ITantrum);

			foreach (var asm in AppDomain.CurrentDomain.GetAssemblies()) {
				if (!ShouldRun) {
					break;
				}

				foreach (var t in asm.GetTypes()) {
					if (!ShouldRun) {
						break;
					}

					if (ti.IsAssignableFrom(t) && t.IsClass) {
						var tantrum = Activator.CreateInstance(t);
						((Tantrums.ITantrum)tantrum).ThrowTantrum(settings, logger);

						logger.Output();
					}
				}
			}

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
