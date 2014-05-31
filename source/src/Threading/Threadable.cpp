#include <Threading/Threadable.h>

namespace Threading
{
	fwvoid Threadable::Sleep(fwuint seconds)
	{
		this->Millisleep(seconds * 1000);

		return;
	}

	fwvoid Threadable::Millisleep(fwuint milliseconds)
	{
#if defined(FW_WINDOWS)
		::Sleep(milliseconds);
#elif defined(FW_UNIX)
		struct timespec req = { 0 }, rem = { 0 };
		req.tv_sec = (fwint)(milliseconds / 1000);
		req.tv_nsec = (milliseconds - (req.tv_sec * 1000)) * 1000000L;

		::nanosleep(&req, &rem);
#endif

		return;
	}

	fwvoid Threadable::Nanosleep(fwulong nanoseconds)
	{
#if defined(FW_WINDOWS)
		fwhandle timer;
		LARGE_INTEGER ft;

		ft.QuadPart = -(10 * nanoseconds);
		timer = CreateWaitableTimer(NULL, TRUE, NULL);
		SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
		CloseHandle(timer);
#elif defined(FW_UNIX)
		struct timespec req = { 0 }, rem = { 0 };
		time_t seconds = (fwint)((nanoseconds - 1000000L) * 1000);
		nanoseconds = nanoseconds - ((seconds * 1000) * 1000000L);

		req.tv_sec = seconds;
		req.tv_nsec = nanoseconds;

		::nanosleep(&req, &rem);
#endif

		return;
	}
}
