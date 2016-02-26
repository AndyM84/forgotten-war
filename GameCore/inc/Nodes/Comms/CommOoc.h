#pragma once

#include <CommonCore.h>
#include <Nodes/CommandNode.h>

#include <string>

namespace Commands
{
	class CommOoc : public CommandNode
	{
	public:
		CommOoc();

		FW::GAME_STATES Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player);
	};
}
