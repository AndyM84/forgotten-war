#include <Nodes/Emotes/EmoteEmote.h>

namespace Commands
{
	EmoteEmote::EmoteEmote()
		: CommandNode("FW::EmoteEmote", "0.0.1")
	{ }

	FW::GAME_STATES EmoteEmote::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		return World.gameState;
	}
}
