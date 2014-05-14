using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;

namespace FW
{
	public class SocketUser
	{
		private IPAddress _Address;
		private int _Port;
		private int _FD;
		private TcpClient _Client;
		private Queue<string> _Messages;

		public IPAddress Address { get { return this._Address; } }
		public int Port { get { return this._Port; } }
		public int FD { get { return this._FD; } }
		public TcpClient Client { get { return this._Client; } }

		public SocketUser(int fd, IPAddress addr, int port, TcpClient client)
		{
			this._FD = fd;
			this._Address = addr;
			this._Port = port;
			this._Client = client;
			this._Messages = new Queue<string>();

			return;
		}

		public void AddMessage(string msg)
		{
			this._Messages.Enqueue(msg);

			return;
		}

		public string GetNextMessage()
		{
			if (this._Messages.Count < 1)
			{
				return null;
			}

			return this._Messages.Dequeue();
		}
	}
}
