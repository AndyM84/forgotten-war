#pragma once

#include <Common/Types.h>

namespace Server
{
	struct SocketConn
	{
		fwuint id;
		SOCKET sock;
		fwchar *buffer;
		fwint totalBytes;
		fwint sentBytes;
		sockaddr_in address;
	};

	struct SocketMessage
	{
		fwstr Message;
		fwuint NumBytes;
	};

	class ServerListener
	{
	public:
		virtual fwvoid ClientConnected(fwuint ID) const = 0;
		virtual fwvoid ClientReceived(fwuint ID, const SocketMessage &Message) const = 0;
		virtual fwvoid ClientDisconnected(fwuint ID) const = 0;
	};
}
