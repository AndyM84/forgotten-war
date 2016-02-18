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

	/* Generic setup routine */
	virtual fwbool Setup();
	/* Generic routine to kill all the things */
	virtual fwbool Destroy();

	/* Libraries::GameLibrary methods */

	/* Main game loop, aka Tick */
	virtual FW::GAME_STATES GameLoop(fwfloat Delta);
	/* Triggers a dump of the current game state into storage */
	virtual fwvoid SaveState();
	/* Receives a list of orphaned clients to try restoring from storage */
	virtual fwvoid RestoreState(std::vector<fwclient> clients);
	/* Provides a link to the CoreArbiter so we can actually interact with some services */
	virtual fwvoid AddArbiter(FW::CoreArbiter &send);
	/* Callback method for when a new client connects */
	virtual fwclient ClientConnected(fwuint ID, const sockaddr_in Address);
	/* Callback method for when a message is received from a client */
	virtual fwclient ClientReceived(fwuint ID, ServerMessage Message);
	/* Callback method fo rwhen a client disconnects */
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
