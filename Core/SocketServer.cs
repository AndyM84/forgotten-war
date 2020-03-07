using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

using Stoic.Log;

namespace FW.Core
{
	public enum CommandTypes
	{
		CONNECTED,
		DISCONNECTED,
		RECEIVED,
		SEND
	}

	public struct IdentifiableSocket
	{
		public int ID { get; set; }
		public Socket Socket { get; set; }
	}

	public struct Command
	{
		public string Body { get; set; }
		public string Contents { get; set; }
		public int ID { get; set; }
		public string Prefix { get; set; }
		public CommandTypes Type { get; set; }
	}

	public class SocketServer
	{
		protected int _CurrentID = 0;
		protected Socket _Listener;
		protected int _ListenPort;
		protected Logger _Logger;
		protected int _MaxConnections;
		protected Dictionary<int, IdentifiableSocket> _Sockets;

		public Dictionary<int, IdentifiableSocket> Sockets { get { return new Dictionary<int, IdentifiableSocket>(this._Sockets); } }


		public SocketServer(int MaxConnections, int ListenPort, ref Logger Logger)
		{
			if (MaxConnections < 5) {
				throw new ArgumentException("Minimum connection limit  must be 5");
			}

			if (ListenPort < 500 || ListenPort > 64000) {
				throw new ArgumentException("Listen port must be between 500 and 64000");
			}

			this._MaxConnections = MaxConnections;
			this._ListenPort = ListenPort;
			this._Logger = Logger;

			this._Sockets = new Dictionary<int, IdentifiableSocket>(MaxConnections);
			this._Listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			IPEndPoint iep = new IPEndPoint(IPAddress.Any, this._ListenPort);
			this._Listener.Bind(iep);
			this._Listener.Listen(MaxConnections / 2);

			return;
		}


		public void Log(LogLevels Level, string Message)
		{
			this._Logger.Log(Level, Message);

			return;
		}

		public List<Command> Poll()
		{
			var ret = new List<Command>();
			List<Socket>
				readSocks = new List<Socket>{ this._Listener },
				errorSocks = new List<Socket>{ this._Listener };

			foreach (var c in this._Sockets) {
				readSocks.Add(c.Value.Socket);
				errorSocks.Add(c.Value.Socket);
			}

			try {
				Socket.Select(readSocks, null, errorSocks, 1000);
			} catch (SocketException se) {
				this.Log(LogLevels.ERROR, "Encountered error while polling socket sets: " + se.ErrorCode);

				return ret;
			}

			if (readSocks.Contains(this._Listener)) {
				this._Sockets.Add(++this._CurrentID, new IdentifiableSocket {
					ID = this._CurrentID,
					Socket = this._Listener.Accept()
				});

				this.Log(LogLevels.INFO, "New client connected, #" + this._CurrentID + " from " + this._Sockets[this._CurrentID].Socket.RemoteEndPoint);
				ret.Add(new Command {
					Contents = this._CurrentID.ToString(),
					ID = this._CurrentID,
					Type = CommandTypes.CONNECTED
				});
			}

			var clientsToRemove = new List<int>();

			foreach (var client in this._Sockets) {
				if (readSocks.Contains(client.Value.Socket)) {
					var data = new byte[4096];
					var recv = client.Value.Socket.Receive(data);

					if (recv > 0) {
						var stringData = Encoding.ASCII.GetString(data, 0, recv).TrimEnd(new char[2] { '\n', '\r' }).Trim();

						if (string.IsNullOrWhiteSpace(stringData)) {
							continue;
						}

						var prefix = stringData.Split(' ')[0];

						this.Log(LogLevels.DEBUG, "Received from client #" + client.Key + ": " + stringData);
						ret.Add(new Command {
							Body = (prefix.Length == stringData.Length) ? stringData : stringData.Substring(prefix.Length).Trim(),
							Contents = stringData,
							ID = client.Key,
							Prefix = prefix,
							Type = CommandTypes.RECEIVED
						});
					} else {
						clientsToRemove.Add(client.Key);

						this.Log(LogLevels.INFO, "Queueing client #" + client.Key + " for disconnection");
						ret.Add(new Command {
							Contents = "DISCONNECT",
							ID = client.Key,
							Type = CommandTypes.DISCONNECTED
						});
					}
				}

				if (errorSocks.Contains(client.Value.Socket)) {
					this.Log(LogLevels.ERROR, "Error on socket for client #" + client.Key + ": " + "");
					ret.Add(new Command {
						Contents = "DISCONNECT",
						ID = client.Key,
						Type = CommandTypes.DISCONNECTED
					});
				}
			}

			if (clientsToRemove.Count > 0) {
				foreach (var id in clientsToRemove) {
					this._Sockets[id].Socket.Close();
					this._Sockets.Remove(id);
				}
			}

			return ret;
		}

		public void Send(int ID, string Buffer)
		{
			if (this._Sockets.ContainsKey(ID)) {
				this._Sockets[ID].Socket.Send(Encoding.ASCII.GetBytes(Buffer));
			}

			return;
		}

		public void Close(int ID)
		{
			if (this._Sockets.ContainsKey(ID)) {
				this._Sockets[ID].Socket.Shutdown(SocketShutdown.Both);
				this._Sockets[ID].Socket.Close();
				this._Sockets.Remove(ID);
			}

			return;
		}

		public void Shutdown()
		{
			foreach (var c in this._Sockets) {
				c.Value.Socket.Close();
				this._Sockets.Remove(c.Key);
			}

			this._Listener.Close();

			return;
		}
	}
}
