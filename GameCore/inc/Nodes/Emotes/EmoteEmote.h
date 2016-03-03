#pragma once

#include <CommonCore.h>
#include <Nodes/CommandNode.h>

namespace Commands
{
	class EmoteEmote : public CommandNode
	{
	public:
		EmoteEmote();

		FW::GAME_STATES Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player);
	};
}
