using System.Net;
using System.Net.Sockets;
using System.Threading;

using Stoic.Log;

namespace DifficultChild.Tantrums
{
	public class BadDisconnect : ITantrum
	{
		public BadDisconnect()
		{
			return;
		}

		public void ThrowTantrum(Settings settings, Logger log)
		{
			try {
				IPHostEntry ipHost = Dns.GetHostEntry(settings.Host);
				IPAddress ipAddr = ipHost.AddressList[0];
				IPEndPoint ep = new IPEndPoint(ipAddr, settings.Port);
				Socket sender = new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

				log.Log(LogLevels.INFO, "Throwing 'BadDisconnect' tantrum");

				sender.Connect(ep);

				Thread.Sleep(1000);

				sender.Close();

				log.Log(LogLevels.INFO, "Finished 'BadDisconnect' tantrum");
			} catch (SocketException sex) {
				log.Log(LogLevels.ERROR, "Error while throwing 'BadDisconnect' tantrum");
				log.Log(LogLevels.DEBUG, sex.Message);
				log.Log(LogLevels.DEBUG, sex.StackTrace);
			}

			return;
		}
	}
}
