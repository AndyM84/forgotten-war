#include <Nodes/Admin/AdminShutdown.h>

namespace Commands
{
	AdminShutdown::AdminShutdown()
		: CommandNode("FW::AdminShutdown", "0.0.1")
	{ }

	FW::GAME_STATES AdminShutdown::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		return FW::GAME_STATES::FWGAME_STOPPING;
	}
}
