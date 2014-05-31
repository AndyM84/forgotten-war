#pragma once

#ifdef _DEBUG
#define FW_DEBUG 1
#endif

#if defined(_WIN32)
#define FW_WINDOWS
#elif defined(_UNIX)
#define FW_UNIX
#endif

#include <string>

typedef bool fwbool;
typedef void fwvoid;

typedef std::wstring wfwstr;
typedef std::string fwstr;

typedef unsigned char fwuchar;
typedef char fwchar;
typedef wchar_t wfwchar;

typedef unsigned int fwuint;
typedef int fwint;

typedef unsigned short fwushort;
typedef short fwshort;

typedef unsigned long fwulong;
typedef long fwlong;

typedef float fwfloat;
typedef double fwdouble;

typedef unsigned short fwword;
typedef unsigned long fwdword;
typedef unsigned long long fwdword64;

#if defined(FW_WINDOWS)
	#include <Windows.h>

	typedef HANDLE fwhandle;
	typedef HINSTANCE fwinstance;
	typedef __int64 fwint64;
	typedef LARGE_INTEGER fwtime;
#elif defined(FW_UNIX)
	typedef void* fwhandle;
	typedef int fwinstance;
	typedef time_t fwint64;
	typedef timespec fwtime;
#endif
