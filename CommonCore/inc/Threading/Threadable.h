#pragma once

#include <Common/Types.h>

namespace Threading
{
	class Threadable
	{
	public:
		fwbool IsValid();
		virtual fwvoid Run();
		fwvoid SignalTerminate();

	protected:
		fwstr m_Name;

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
