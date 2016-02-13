#pragma once

#include <CommonCore.h>

#include <queue>

class Player
{
public:
	Player(const fwuint PlayerID, const fwuint ClientID, const sockaddr_in Address, const ConnectedClientStates State);

	// Getters
	const fwclient GetClient() const;
	const fwuint GetID() const;
	std::shared_ptr<ServerMessage> GetNextMessage();

	// Actions
	fwvoid AddBufferMessage(std::shared_ptr<ServerMessage> Message);

protected:
	std::queue<std::shared_ptr<ServerMessage>> buffer;
	const fwclient client;
	const fwuint id;
};
