#pragma once

#include <CommonCore.h>
#include <Player.h>

struct World
{
	std::map<fwuint, std::shared_ptr<Player>> players;
	FW::GAME_STATES gameState;
};
