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

	virtual fwvoid ClientConnected(fwuint ID) const
	{

	}

	virtual fwvoid ClientReceived(fwuint ID, const Server::SocketMessage &Message) const
	{

	}

	virtual fwvoid ClientDisconnected(fwuint ID) const
	{

	}

protected:
	Logging::Logger *logger;
	Server::SelectServer server;

	fwvoid log(const Logging::LogLevel Level, const fwchar *Message)
	{
		if (this->logger)
		{
			this->logger->Log(Message, Level);
		}

		return;
	}
};
