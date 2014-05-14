using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace FWServ
{
	public class SocketServer
	{
		private Dictionary<int, Descriptor> Descriptors;
		private int AutoInc = 0;

		private Socket Listener;
		private bool Running;

		public SocketServer()
		{
			this.Running = false;

			var ipep = new IPEndPoint(IPAddress.Any, 9000);

			this.Listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			this.Listener.Bind(ipep);
			this.Listener.Listen(64);
			this.Listener.BeginAccept(new AsyncCallback(AcceptConnection), this.Listener);

			return;
		}

		private void AcceptConnection(IAsyncResult ares)
		{

		}

		private void ReceiveData(IAsyncResult ares)
		{

		}
	}
}
