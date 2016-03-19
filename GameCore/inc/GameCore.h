#pragma once

#include <CommonCore.h>
#include <Nodes/Commands.h>
#include <World.h>

#include <iostream>
#include <map>

#define _SQLNCLI_OLEDB_
#include <sqlncli.h>

// Utility macros
#define UMIN(a, b)               ((a) < (b) ? (a) : (b))
#define UMAX(a, b)               ((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)          ((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define IS_SET(flag, bit)        ((flag) & (bit))
#define SET_BIT(var, bit)        ((var) |= (bit))
#define REMOVE_BIT(var, bit)     ((var) &= ~(bit))

// Color codes
#define GREY                     "`w"
#define WHITE                    "`W"
#define GREEN                    "`G"
#define BLUE                     "`B"
#define RED                      "`R"
#define CYAN                     "`C"
#define YELLOW                   "`Y"
#define MAGENTA                  "`M"
#define DARK_GREEN               "`g"
#define DARK_BLUE                "`b"
#define DARK_RED                 "`r"
#define DARK_CYAN                "`c"
#define DARK_YELLOW              "`y"
#define DARK_MAGENTA             "`m"
#define DARK_GREY                "`K"
#define BLACK                    "`k"

class GameCore : public Libraries::GameLibrary
{
public:
	/* Destructor */
	~GameCore();

	/* Libraries::Library methods */

	/* Generic setup routine */
	virtual fwbool Setup();
	/* Setup that receives a game config */
	virtual fwbool Setup(const GameConfig &config);
	/* Generic routine to kill all the things */
	virtual fwbool Destroy();

	/* Libraries::GameLibrary methods */

	/* Main game loop, aka Tick */
	FW::GAME_STATES GameLoop(fwfloat Delta);
	/* Triggers a dump of the current game state into storage */
	fwvoid SaveState();
	/* Receives a list of orphaned clients to try restoring from storage */
	fwvoid RestoreState(std::vector<fwclient> clients);
	/* Provides a link to the CoreArbiter so we can actually interact with some services */
	fwvoid AddArbiter(FW::CoreArbiter &send);
	/* Callback method for when a new client connects */
	fwclient ClientConnected(fwuint ID, const sockaddr_in Address);
	/* Callback method for when a message is received from a client */
	fwclient ClientReceived(fwuint ID, ServerMessage Message);
	/* Callback method fo rwhen a client disconnects */
	fwclient ClientDisconnected(fwuint ID, const sockaddr_in Address);
	/* Allows a log message to be recorded */
	fwvoid Log(const Logging::LogLevel Level, const fwstr Message);
	/* Sends a message to a single client */
	fwvoid SendToClient(const fwclient Client, const fwstr Message);
	/* Closes a client connection */
	fwvoid CloseClient(const fwclient Client);
	/* Broadcasts a string to all players save the one provided */
	fwvoid BroadcastToAllButPlayer(const fwclient Client, const fwstr Message);
	/* Broadcasts a string to all players */
	fwvoid BroadcastToAll(const fwstr Message);

	/* GameCore methods */
	std::shared_ptr<Player> GameCore::GetPlayerBySocket(fwuint SockFD);

protected:
	typedef std::pair<fwstr, CommandNode*> commandPair;

	std::map<fwstr, CommandNode*> commands;
	Threading::LockCriticalSection playerLock;
	World world;

	fwstr doColor(const fwstr original);
	fwstr doPrompt(std::shared_ptr<Player> Player, const fwstr original);
};
