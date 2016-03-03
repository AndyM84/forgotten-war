#pragma once

#include <CommonCore.h>
#include <Nodes/CommandNode.h>

namespace Commands
{
	class CommSay : public CommandNode
	{
	public:
		CommSay();

		FW::GAME_STATES Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player);
	};
}
