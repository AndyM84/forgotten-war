#pragma once

#include <CommonCore.h>
#include <Nodes/CommandNode.h>

namespace Commands
{
	class CommYell : public CommandNode
	{
	public:
		CommYell();

		FW::GAME_STATES Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player);
	};
}
