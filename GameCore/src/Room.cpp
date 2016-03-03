#include <Room.h>

Room::Room(fwint X, fwint Y, fwint Z)
	: Position { X, Y, Z }
{ }

Room::Room(Vector Position)
	: Position(Position)
{ }
