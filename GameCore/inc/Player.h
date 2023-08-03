#pragma once

#include <CommonCore.h>
#include <Room.h>

#include <queue>
#include <math.h>

#define PLAYER_IDLE_START 120000.0f
#define PLAYER_IDLE_LIMIT 500000.0f

enum PLAYER_STATES
{
	PLAYER_INVALID,
	PLAYER_CONNECTING,
	PLAYER_AWAITINGNAME,
	PLAYER_CONNECTED,
	PLAYER_IDLE,
	PLAYER_DISCONNECTED
};

class Player
{
public:
	Player(const fwuint PlayerID, const fwuint ClientID, const sockaddr_in Address, const PLAYER_STATES State);

	// Getters
	const fwclient GetClient() const;
	const fwuint GetID() const;
	std::shared_ptr<ServerMessage> GetNextMessage();
	const fwstr GetName() const;
	const PLAYER_STATES GetState() const;
	const Vector GetLocation() const;
	const fwfloat GetIdleTime() const;
	fwbool IsInLocation(Vector Location) const;
	fwbool IsNearLocation(Vector Location) const;

	// Actions
	Player &AddBufferMessage(std::shared_ptr<ServerMessage> Message);
	Player &SetName(fwstr Name);
	Player &SetState(PLAYER_STATES State);
	Player &SetLocation(Vector Location);
	Player &AddIdleTime(fwfloat Time);

protected:
	std::queue<std::shared_ptr<ServerMessage>> buffer;
	PLAYER_STATES state;
	fwfloat idleTime;
	fwclient client;
	const fwuint id;
	Vector location;
	fwstr name;
};