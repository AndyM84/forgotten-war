#include <Player.h>

Player::Player(const fwuint PlayerID, const fwuint ClientID, const sockaddr_in Address, const ConnectedClientStates State)
	: client(fwclient { ClientID, PlayerID, Address, State }), id(PlayerID)
{
	switch (State)
	{
	case CCLIENT_CONNECTED:
		this->SetState(PLAYER_CONNECTED);
		break;
	case CCLIENT_CONNECTING:
		this->SetState(PLAYER_CONNECTING);
		break;
	case CCLIENT_DISCONNECTED:
		this->SetState(PLAYER_DISCONNECTED);
		break;
	case CCLIENT_INVALID:
	default:
		this->SetState(PLAYER_INVALID);
		break;
	}

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

// Actions
Player &Player::AddBufferMessage(const std::shared_ptr<ServerMessage> Message)
{
	this->buffer.push(Message);

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
