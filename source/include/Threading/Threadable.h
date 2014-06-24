#pragma once

#include <Common/Types.h>

namespace Threading
{
	class Threadable
	{
	public:
		virtual fwvoid Run();
		fwvoid SignalTerminate();

	protected:
		Threadable() { };
		~Threadable() { };

		virtual fwvoid Tick();

		fwvoid Sleep(fwuint seconds);
		fwvoid Millisleep(fwuint milliseconds);
		fwvoid Nanosleep(fwulong nanoseconds);

		fwbool IsRunning();

	private:
		fwbool m_Running;
	};
}
