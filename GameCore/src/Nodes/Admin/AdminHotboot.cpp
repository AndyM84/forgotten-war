#include <Nodes/Admin/AdminHotboot.h>

namespace Commands
{
	AdminHotboot::AdminHotboot()
		: CommandNode("FW::AdminHotboot", "0.0.1")
	{ }

	FW::GAME_STATES AdminHotboot::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		return FW::GAME_STATES::FWGAME_HOTBOOTING;
	}
}
