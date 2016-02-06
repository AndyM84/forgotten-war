#pragma once

/* Standard includes */
#include <exception>
#include <iostream>

/* FW includes */
#include <Common/Types.h>
#include <Logging/Logger.h>
#include <Logging/ConsoleAppender.h>
#include <Logging/FileAppender.h>
#include <Threading/Thread.h>
#include <Threading/LockCriticalSection.h>
#include <Threading/Threadable.h>
#include <Server/SelectServer.h>

class ForgottenWar : public Server::ServerListener
{
public:
	ForgottenWar(fwint Port)
		: server(*this, Port)
	{ }

	ForgottenWar(fwint Port, Logging::Logger &Logger)
		: server(*this, Port, Logger), logger(&Logger)
	{ }

	~ForgottenWar()
	{

	}

	fwvoid Start()
	{
		this->log(Logging::LogLevel::LOG_DEBUG, "Starting server.");

		this->server.Initialize();

		auto st = Threading::Thread(this->server);
		st.Start();

		std::cin.get();

		st.Terminate();

		return;
	}

	virtual fwvoid ClientConnected(fwuint ID, const sockaddr_in Address)
	{
		std::stringstream ss;
		ss << "New client connected from: " << inet_ntoa(Address.sin_addr);

		this->log(Logging::LogLevel::LOG_TRACE, ss.str().c_str());

		this->server.Send(ID, "This is a test message!\n");

		return;
	}

	virtual fwvoid ClientReceived(fwuint ID, const Server::SocketMessage &Message)
	{
		std::stringstream ss;
		ss << "Received message from user: " << Message.Message;

		this->log(Logging::LogLevel::LOG_INFO, ss.str().c_str());

		return;
	}

	virtual fwvoid ClientDisconnected(fwuint ID, const sockaddr_in Address)
	{
		std::stringstream ss;
		ss << "Client disconnected: " << inet_ntoa(Address.sin_addr);

		this->log(Logging::LogLevel::LOG_WARN, ss.str().c_str());

		return;
	}

protected:
	Logging::Logger *logger;
	Server::SelectServer server;

	fwvoid log(Logging::LogLevel Level, const fwchar *Message)
	{
		if (this->logger)
		{
			this->logger->Log(Message, Level);
		}

		return;
	}
};
