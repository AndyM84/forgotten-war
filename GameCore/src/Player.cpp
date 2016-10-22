#include <Player.h>

Player::Player(const fwuint PlayerID, const fwuint ClientID, const sockaddr_in Address, const PLAYER_STATES State)
	: client(fwclient { ClientID, PlayerID, Address, CCLIENT_INVALID }), id(PlayerID)
{
	this->SetState(State);
	this->SetLocation(Vector { 0, 0, 0 });

	return;
}

// Getters
const fwclient Player::GetClient() const
{
	return this->client;
}

const fwuint Player::GetID() const
{
	return this->id;
}

std::shared_ptr<ServerMessage> Player::GetNextMessage()
{
	if (this->buffer.empty())
	{
		return NULL;
	}

	auto front = this->buffer.front();
	this->buffer.pop();

	return front;
}

const fwstr Player::GetName() const
{
	return this->name;
}

const PLAYER_STATES Player::GetState() const
{
	return this->state;
}

const Vector Player::GetLocation() const
{
	return this->location;
}

const fwfloat Player::GetIdleTime() const
{
	return this->idleTime;
}

fwbool Player::IsInLocation(Vector Location) const
{
	if (Location.X == this->location.X && Location.Y == this->location.Y && Location.Z == this->location.Z)
	{
		return true;
	}

	return false;
}

fwbool Player::IsNearLocation(Vector Location) const
{
	Vector dist = { abs(Location.X - this->location.X), abs(Location.Y - this->location.Y), abs(Location.Z - this->location.Z) };

	return dist.X < NEAR_DISTANCE && dist.Y < NEAR_DISTANCE && dist.Z < NEAR_DISTANCE;
}

// Actions
Player &Player::AddBufferMessage(const std::shared_ptr<ServerMessage> Message)
{
	this->buffer.push(Message);
	this->idleTime = 0.0f;

	return *this;
}

Player &Player::SetName(fwstr Name)
{
	this->name = Name;

	return *this;
}

Player &Player::SetState(PLAYER_STATES State)
{
	this->state = State;

	switch (this->state)
	{
	case PLAYER_CONNECTING:
	case PLAYER_AWAITINGNAME:
		this->client.state = CCLIENT_CONNECTING;
		break;
	case PLAYER_CONNECTED:
		this->client.state = CCLIENT_CONNECTED;
		break;
	case PLAYER_DISCONNECTED:
		this->client.state = CCLIENT_DISCONNECTED;
		break;
	case PLAYER_INVALID:
	default:
		this->client.state = CCLIENT_INVALID;
		break;
	}

	return *this;
}

Player &Player::SetLocation(Vector Location)
{
	this->location = Location;

	return *this;
}

Player &Player::AddIdleTime(fwfloat Time)
{
	this->idleTime += Time;

	return *this;
}
