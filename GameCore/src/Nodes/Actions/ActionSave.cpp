#include <Nodes/Actions/ActionSave.h>

namespace Commands
{
	ActionSave::ActionSave()
		: CommandNode("FW::ActionSave", "0.0.1")
	{ }

	FW::GAME_STATES ActionSave::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		return World.gameState;
	}
}
