#include <Nodes/Actions/ActionMove.h>

namespace Commands
{
	ActionMove::ActionMove()
		: CommandNode("FW::ActionMove", "0.0.1")
	{ }

	FW::GAME_STATES ActionMove::Process(Libraries::GameLibrary &Sender, World World, std::shared_ptr<ServerMessage> Message, std::shared_ptr<Player> Player)
	{
		fwbool willMove = false;
		auto cmd = Message->GetCmd();
		fwstr arrivDir = "";
		Vector nLoc = Player->GetLocation();

		fwint MaxDist = 5, MinDist = -5;

		if (cmd == "north" && nLoc.Y < MaxDist) {
			arrivDir = "the south";
			willMove = true;
			nLoc.Y++;
		} else if (cmd == "south" && nLoc.Y > MinDist) {
			arrivDir = "the north";
			willMove = true;
			nLoc.Y--;
		} else if (cmd == "east" && nLoc.X < MaxDist) {
			arrivDir = "the west";
			willMove = true;
			nLoc.X++;
		} else if (cmd == "west" && nLoc.X > MinDist) {
			arrivDir = "the east";
			willMove = true;
			nLoc.X--;
		} else if (cmd == "up" && nLoc.Z < MaxDist) {
			arrivDir = "below";
			willMove = true;
			nLoc.Z++;
		} else if (cmd == "down" && nLoc.Z > MinDist) {
			arrivDir = "above";
			willMove = true;
			nLoc.Z--;
		}

		if (willMove) {
			for (auto plr : World.players) {
				if (plr.first == Player->GetID()) {
					continue;
				} else if (plr.second->IsInLocation(Player->GetLocation())) {
					std::stringstream ss;
					ss << Player->GetName() << " has left " << cmd << ".";
					Sender.SendToClient(plr.second->GetClient(), ss.str());
				} else if (plr.second->IsInLocation(nLoc)) {
					std::stringstream ss;
					ss << Player->GetName() << " has arrived from " << arrivDir << ".";
					Sender.SendToClient(plr.second->GetClient(), ss.str());
				}
			}

			Player->SetLocation(nLoc);

			fwbool hasUsers = false;
			std::stringstream ss;
			ss << "A Room\n-----------------\nYou are in a room, just like every other.";

			for (auto plr : World.players) {
				if (plr.second->IsInLocation(Player->GetLocation())) {
					if (!hasUsers) {
						ss << "\n";
						hasUsers = true;
					}

					ss << "\n";
					ss << ((plr.first == Player->GetID()) ? "You" : plr.second->GetName());
					ss << ((plr.first == Player->GetID()) ? " are here." : " is here.");
				}
			}

			Sender.SendToClient(Player->GetClient(), ss.str());
		} else {
			Sender.SendToClient(Player->GetClient(), "You can't move that direction!");
		}

		return World.gameState;
	}
}
