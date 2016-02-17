#pragma once

#include <CommonCore.h>
#include <Player.h>

#include <iostream>
#include <map>

// Utility macros
#define UMIN(a, b)               ((a) < (b) ? (a) : (b))
#define UMAX(a, b)               ((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)          ((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define IS_SET(flag, bit)        ((flag) & (bit))
#define SET_BIT(var, bit)        ((var) |= (bit))
#define REMOVE_BIT(var, bit)     ((var) &= ~(bit))

class GameCore : public Libraries::GameLibrary
{
public:
	/* Destructor */
	~GameCore();

	/* Libraries::Library methods */
	virtual fwbool Setup();
	virtual fwbool Destroy();

	/* Libraries::GameLibrary methods */
	virtual FW::GAME_STATES GameLoop(fwfloat Delta);
	virtual fwvoid SaveState();
	virtual fwvoid RestoreState(std::vector<fwclient> clients);
	virtual fwvoid AddArbiter(FW::CoreArbiter &send);
	virtual fwclient ClientConnected(fwuint ID, const sockaddr_in Address);
	virtual fwclient ClientReceived(fwuint ID, ServerMessage Message);
	virtual fwclient ClientDisconnected(fwuint ID, const sockaddr_in Address);

	/* GameCore methods */
	fwvoid Log(const Logging::LogLevel Level, const fwchar *Message);
	fwvoid SendToClient(const fwclient Client, const fwstr Message);
	fwvoid CloseClient(const fwclient Client);
	fwvoid BroadcastToAllButPlayer(const std::shared_ptr<Player> Client, const fwstr Message);
	fwvoid BroadcastToAll(const fwstr Message);
	std::vector<fwclient> GetClients();

protected:
	std::map<fwuint, std::shared_ptr<Player>> players;
	Threading::LockMutex playerLock;
	Threading::Thread *gameThread;
};
