#pragma once

#include <Common/Types.h>

namespace Threading
{
	class Threadable
	{
	public:
		fwvoid Run();
		fwvoid SignalTerminate();

	protected:
		Threadable() { };
		~Threadable() { };

		virtual fwvoid Tick() = 0;

		fwvoid Sleep(fwuint seconds);
		fwvoid Millisleep(fwuint milliseconds);
		fwvoid Nanosleep(fwulong nanoseconds);

		fwbool IsRunning();

	private:
		fwbool m_Running;
	};
}