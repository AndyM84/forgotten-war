using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace FW
{
	public class SocketServer
	{
		public List<SocketUser> Users { get; }
		public List<Thread> clientThreads { get; set; }

		private TcpListener listener;
		private int clientsPerThread;
		private Thread listenThread;
		private IPAddress bindAddr;
		private Thread readThread;
		private int maxClients;
		private bool running;
		private int port;

		public SocketServer(int port, IPAddress bindAddr, int maxClients)
		{
			this.maxClients = maxClients;
			this.clientsPerThread = 25;
			this.bindAddr = bindAddr;
			this.running = true;
			this.port = port;

			return;
		}

		public void Start()
		{
			this.listener = new TcpListener(this.bindAddr, this.port);

			this.listenThread = new Thread(new ThreadStart(this.ListenForClients));
			this.listenThread.Start();

			return;
		}

		public void Stop()
		{
			this.running = false;

			this.listenThread.Join();

			return;
		}

		private void ListenForClients()
		{
			this.listener.Start();

			return;
		}
	}
}
