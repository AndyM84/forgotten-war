#pragma once

#include <CommonCore.h>

#define NEAR_DISTANCE 3

struct Vector
{
	fwint X, Y, Z;
};

class Room
{
public:
	const Vector Position;

	Room(fwint X, fwint Y, fwint Z);
	Room(Vector Position);
};
