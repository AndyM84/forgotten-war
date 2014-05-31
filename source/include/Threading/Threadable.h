#pragma once

#include <Common/Types.h>

namespace Threading
{
	class Threadable
	{
	public:
		virtual void Run() = 0;

	protected:
		Threadable() { };
		~Threadable() { };

		fwvoid Sleep(fwuint seconds);
		fwvoid Millisleep(fwuint milliseconds);
		fwvoid Nanosleep(fwulong nanoseconds);
	};
}
