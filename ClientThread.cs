using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;

namespace FW
{
	public class ClientThread
	{
		private List<TcpClient> Clients;

		public ClientThread()
		{
			this.Clients = new List<TcpClient>();

			return;
		}

		public int GetClientCount()
		{
			return this.Clients.Count;
		}

		public void AddClient(TcpClient client)
		{
			this.Clients.Add(client);

			return;
		}

		public void ThreadWorker()
		{
			// need to figure out how we want to carry messages between the SocketUser stuff and the ClientThreads so it can be delivered

			return;
		}
	}
}
