#include <Server/SelectServer.h>

namespace Server
{
	SelectServer::SelectServer(const std::shared_ptr<ServerListener> Listener, fwint Port)
		: Listener(Listener), port(Port), isInitialized(false), shouldRun(true)
	{ }

	SelectServer::SelectServer(const std::shared_ptr<ServerListener> Listener, fwint Port, const std::shared_ptr<Logging::Logger> Logger)
		: Listener(Listener), port(Port), Logger(Logger), isInitialized(false), shouldRun(true)
	{ }

	SelectServer::~SelectServer()
	{
		if (this->isInitialized)
		{
			this->log(Logging::LogLevel::LOG_DEBUG, "SelectServer - Shutting down listener socket and calling WSACleanup()");

			closesocket(this->listenSocket);
			WSACleanup();
		}

		return;
	}

	fwvoid SelectServer::Run()
	{
		if (!this->isInitialized)
		{
			this->log(Logging::LogLevel::LOG_CRITICAL, "SelectServer - Socket system was not initialized, cannot run");

			return;
		}

		while (this->shouldRun)
		{
			this->initSets();

			if (select(0, &this->setRead, &this->setWrite, &this->setExcept, 0) > 0)
			{
				if (FD_ISSET(this->listenSocket, &this->setRead))
				{
					sockaddr_in clientAddr;
					fwint clientLength = sizeof(clientAddr);

					SOCKET sck = accept(this->listenSocket, (sockaddr*)&clientAddr, &clientLength);

					if (sck == INVALID_SOCKET)
					{
						std::stringstream ss("SelectServer - Error while accepting a socket: ");
						ss << this->getSocketError(this->listenSocket);

						this->log(Logging::LogLevel::LOG_ERROR, ss.str().c_str());

						continue;
					}

					std::stringstream ss("SelectServer - Client connected from: ");
					ss << inet_ntoa(clientAddr.sin_addr);

					this->log(Logging::LogLevel::LOG_INFO, ss.str().c_str());

					u_long noBlock = 1;
					ioctlsocket(sck, FIONBIO, &noBlock);

					this->lock.Block();
					this->addClient(sck, clientAddr);
					this->lock.Release();
				}

				if (FD_ISSET(this->listenSocket, &this->setExcept))
				{
					std::stringstream ss("SelectServer - Error while accepting socket: ");
					ss << this->getSocketError(this->listenSocket);

					this->log(Logging::LogLevel::LOG_ERROR, ss.str().c_str());

					continue;
				}

				if (this->clients.size() > 0)
				{
					this->lock.Block();
					std::vector<int> idsToRemove;

					for (auto client : this->clients)
					{
						if (FD_ISSET(client.sock, &this->setRead))
						{
							fwint bytes = recv(client.sock, client.buffer, 4096, 0);

							if (bytes == 0 || bytes == SOCKET_ERROR)
							{
								if (bytes != 0)
								{
									std::stringstream ss("SelectServer - Error while receiving on a socket: ");
									ss << this->getSocketError(client.sock);

									this->log(Logging::LogLevel::LOG_ERROR, ss.str().c_str());

									continue;
								}

								idsToRemove.push_back(client.id);

								continue;
							}

							client.totalBytes = bytes;
							client.sentBytes = 0;

							this->Listener->ClientReceived(client.id, SocketMessage { client.buffer, (fwuint)bytes });
						}

						if (FD_ISSET(client.sock, &this->setWrite))
						{
							fwint bytes = 0;

							if ((client.totalBytes - client.sentBytes) > 0)
							{
								bytes = send(client.sock, client.buffer + client.sentBytes, client.totalBytes - client.sentBytes, 0);

								if (bytes == SOCKET_ERROR)
								{
									std::stringstream ss("SelectServer - Error while sending on a socket: ");
									ss << this->getSocketError(client.sock);

									this->log(Logging::LogLevel::LOG_ERROR, ss.str().c_str());

									continue;
								}

								if (bytes == (client.totalBytes - client.sentBytes))
								{
									client.totalBytes = 0;
									client.sentBytes = 0;
								}
								else
								{
									client.sentBytes += bytes;
								}
							}
						}

						if (FD_ISSET(client.sock, &this->setExcept))
						{
							std::stringstream ss("SelectServer - Error on a socket: ");
							ss << this->getSocketError(client.sock);

							this->log(Logging::LogLevel::LOG_ERROR, ss.str().c_str());

							continue;
						}
					}

					if (idsToRemove.size() > 0)
					{
						for (auto id : idsToRemove)
						{
							for (size_t i = 0; i < this->clients.size(); ++i)
							{
								for (auto b = this->clients.begin(); b != this->clients.end(); ++b)
								{
									if ((*b).id == id)
									{
										this->clients.erase(b);

										break;
									}
								}
							}
						}
					}

					this->lock.Release();
				}
			}
		}

		return;
	}

	fwvoid SelectServer::Initialize()
	{
		this->log(Logging::LogLevel::LOG_DEBUG, "SelectServer - Beginning startup procedure");

		if (!this->Listener)
		{
			this->log(Logging::LogLevel::LOG_CRITICAL, "SelectServer - No ServerListener provided, startup cannot complete");

			return;
		}

		fwint nResult;
		WSADATA wsaData;

		this->log(Logging::LogLevel::LOG_DEBUG, "SelectServer - Calling WSAStartup()");
		nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (nResult != NO_ERROR)
		{
			this->log(Logging::LogLevel::LOG_CRITICAL, "SelectServer - Failed startup; Error during WSAStartup()");

			return;
		}

		this->log(Logging::LogLevel::LOG_DEBUG, "SelectServer - Initializing listener socket");
		this->listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (this->listenSocket == INVALID_SOCKET)
		{
			this->log(Logging::LogLevel::LOG_CRITICAL, "SelectServer - Error while opening listener socket");
			WSACleanup();

			return;
		}

		struct sockaddr_in serverAddr;
		ZeroMemory((char *)&serverAddr, sizeof(serverAddr));

		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(this->port);

		this->log(Logging::LogLevel::LOG_DEBUG, "SelectServer - Binding listener socket");

		if (bind(this->listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			this->log(Logging::LogLevel::LOG_CRITICAL, "SelectServer - Error while binding listener socket");
			WSACleanup();

			return;
		}

		this->log(Logging::LogLevel::LOG_DEBUG, "SelectServer - Setting listener socket as a listening socket");

		if (listen(this->listenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			this->log(Logging::LogLevel::LOG_CRITICAL, "SelectServer - Error while trying to listen");
			WSACleanup();

			return;
		}

		this->isInitialized = true;

		return;
	}

	fwvoid SelectServer::Send(const fwuint ID, fwstr Message)
	{
		this->lock.Block();

		for (auto client : this->clients)
		{
			if (client.id == ID)
			{
				if (strlen(client.buffer) > 0)
				{
					fwstr buffer = client.buffer;
					buffer += Message;

					client.buffer = const_cast<fwchar*>(buffer.c_str());
				}
				else
				{
					client.buffer = const_cast<fwchar*>(Message.c_str());
				}

				client.totalBytes = strlen(client.buffer) + 1;

				break;
			}
		}

		this->lock.Release();

		return;
	}

	fwvoid SelectServer::Close(const fwuint ID)
	{
		this->lock.Block();

		for (auto iter = this->clients.begin(); iter != this->clients.end(); ++iter)
		{
			if ((*iter).id == ID)
			{
				closesocket((*iter).sock);
				this->clients.erase(iter);

				break;
			}
		}

		this->lock.Release();

		return;
	}

	fwvoid SelectServer::Stop()
	{
		return;
	}

	fwvoid SelectServer::initSets()
	{
		if (!this->isInitialized)
		{
			return;
		}

		this->log(Logging::LogLevel::LOG_DEBUG, "SelectServer - (Re)Initialize file descriptor sets");

		FD_ZERO(&this->setRead);
		FD_ZERO(&this->setWrite);
		FD_ZERO(&this->setExcept);

		this->log(Logging::LogLevel::LOG_DEBUG, "SelectServer - Adding listener socket to read/except file descriptor sets");

		FD_SET(this->listenSocket, &this->setRead);
		FD_SET(this->listenSocket, &this->setExcept);

		if (this->clients.size() > 0)
		{
			this->log(Logging::LogLevel::LOG_DEBUG, "SelectServer - Assigning known client sockets to file descriptor sets");

			this->lock.Block();

			for (auto client : this->clients)
			{
				if (client.sentBytes < client.totalBytes)
				{
					FD_SET(client.sock, &this->setWrite);
				}
				else
				{
					FD_SET(client.sock, &this->setRead);
				}

				FD_SET(client.sock, &this->setExcept);
			}

			this->lock.Release();
		}

		return;
	}

	fwvoid SelectServer::log(const Logging::LogLevel Level, const fwchar *Message)
	{
		if (this->Logger)
		{
			this->Logger->Log(Message, Level);
		}

		return;
	}

	fwint SelectServer::getSocketError(SOCKET socket)
	{
		fwint optValue, optValueLength = sizeof(optValue);

		getsockopt(socket, SOL_SOCKET, SO_ERROR, (char *)&optValue, &optValueLength);

		return optValue;
	}

	fwvoid SelectServer::addClient(SOCKET socket, sockaddr_in address)
	{
		fwuint nId = 0;
		SocketConn conn;
		conn.sock = socket;
		conn.address = address;

		for (auto client : this->clients)
		{
			if (client.id >= nId)
			{
				nId = client.id + 1;
			}
		}

		conn.id = nId;

		this->clients.push_back(conn);

		return;
	}
}
