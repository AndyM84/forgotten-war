#pragma once

#include <CommonCore.h>
#include <Player.h>
#include <Room.h>

struct World
{
	std::map<fwuint, std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<Room>> rooms;
	FW::GAME_STATES gameState;
};
