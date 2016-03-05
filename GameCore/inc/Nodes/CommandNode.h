#pragma once

#include <CommonCore.h>
#include <World.h>

class CommandNode : public N2f::NodeBase<ServerMessage>
{
public:
	CommandNode(fwstr Key, fwstr Version) : N2f::NodeBase<ServerMessage>(Key.c_str(), Version.c_str()) { }
	CommandNode(const CommandNode &original) = default;
	CommandNode(CommandNode &&original) = default;
	virtual ~CommandNode() { }

	virtual void Process(void *Sender, std::shared_ptr<ServerMessage> Dispatch) { };
	virtual FW::GAME_STATES Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player) = 0;
};
