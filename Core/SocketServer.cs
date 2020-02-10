using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace FW.Core
{
	public class SocketServer
	{
		/// Set this up to contain sockets internally and return only references to socket ID's with 'commands'
		///   (eg. 'connected', 'disconnect', 'recvd', etc)
		/// Make methods that can send/receive 'commands' that are then processed by the 'poll' method
		/// 
		/// Methods:
		///   - Ctor: initializes with max connection count and host settings
		///   - Poll: does one pass on 'select' and returns listen sets as commands, returns tuple of 'status' and List<command>
		///   - Send: receives a command to send to a socket by its ID ('close' is a command)
	}
}
