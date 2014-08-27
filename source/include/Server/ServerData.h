#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <vector>
#include <iostream>

extern "C" {
	#include <Server/dyad.h>
};


namespace Server
{
	struct Character
	{
		fwstr Username;
		time_t Connected;
		fwbool IsRegistered;
		dyad_Stream *Stream;
	};

	typedef std::vector<Character *> CharList;
	typedef std::vector<Character *>::iterator CharIterator;
}