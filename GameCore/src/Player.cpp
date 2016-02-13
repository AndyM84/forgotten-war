#include <Player.h>

Player::Player(const fwuint PlayerID, const fwuint ClientID, const sockaddr_in Address, const ConnectedClientStates State)
	: client(fwclient { ClientID, PlayerID, Address, State }), id(PlayerID)
{
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

// Actions
fwvoid Player::AddBufferMessage(const std::shared_ptr<ServerMessage> Message)
{
	this->buffer.push(Message);

	return;
}
