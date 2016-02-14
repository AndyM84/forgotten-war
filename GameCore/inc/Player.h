#pragma once

#include <CommonCore.h>

#include <queue>

enum PLAYER_STATES
{
	PLAYER_INVALID,
	PLAYER_CONNECTING,
	PLAYER_AWAITINGNAME,
	PLAYER_CONNECTED,
	PLAYER_DISCONNECTED
};

class Player
{
public:
	Player(const fwuint PlayerID, const fwuint ClientID, const sockaddr_in Address, const ConnectedClientStates State);

	// Getters
	const fwclient GetClient() const;
	const fwuint GetID() const;
	std::shared_ptr<ServerMessage> GetNextMessage();
	const fwstr GetName() const;
	const PLAYER_STATES GetState() const;

	// Actions
	Player &AddBufferMessage(std::shared_ptr<ServerMessage> Message);
	Player &SetName(fwstr Name);
	Player &SetState(PLAYER_STATES State);

protected:
	std::queue<std::shared_ptr<ServerMessage>> buffer;
	PLAYER_STATES state;
	fwclient client;
	const fwuint id;
	fwstr name;
};
