#ifndef UTIL_H
#define UTIL_H

#define PROJECT_NAME ((char*)"Snot")

#define MIN(a, b) (b + ((a - b) & (a - b) >> 31))
#define MAX(a, b) (a - ((a - b) & (a - b) >> 31))
#define ABS(x) ((x) < 0 ? -(x) : (x))

#define PI (3.14159265359f)
#define DEG2RAD(d) ((d) * ((game_state.settings.pi) * .00555556f))
#define RAD2DEG(d) ((180.f / game_state.settings.pi) * (d))

#define KILOBYTE (1024)

#define PROGRAM_SUCCESS (0)
#define PROGRAM_FAIL (-1)

#if defined(_WIN32) || defined(_WIN64)
#ifndef OS_WINDOWS
#define OS_WINDOWS
#endif /* OS_WINDOWS */
#elif defined(__APPLE__) && defined(__MACH__)
#ifndef OS_OSX
#define OS_OSX
#endif /* OS_OSX */
#elif defined(__unix__)
#ifndef OS_UNIX
#define OS_UNIX
#endif /* OS_UNIX */
#if defined(__linux__)
#ifndef OS_LINUX
#define OS_LINUX
#endif /* OS_LINUX */
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#ifndef OS_FREEBSD
#define OS_FREEBSD
#endif /* OS_FREEBSD */
#else
#warning Unknown Unix.
#endif
#else
#warning Unknown OS.
#endif

#ifdef OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#define MAIN S32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, S32 nShowCmd)
#else
#define MAIN S32 main(S32 argc, char** argv)
#endif /* OS_WINDOWS */

#define loop for(;;)
#define let const
#define mut
#define elif else if
#define discard (U0)
#define default_initialiser {0}
#define fallthrough

#ifndef true
#define true (1)
#endif /* true */
#ifndef false
#define false (0)
#endif /* false */

#ifdef DEBUG_BUILD
#define ASSERT(cond) \
	if (!(cond)) { \
		printf("Assert failed: %s, file %s, line %d\n", \
		#cond, __FILE__, __LINE__); exit(PROGRAM_FAIL); }
#else
#define ASSERT(cond) discard (cond)
#endif /* DEBUG_BUILD */

/* Technically this is bad practise, because shorts and chars might be different sized on different systems, but I don't care. */
typedef unsigned char U8;
typedef signed char S8;

typedef unsigned short U16;
typedef signed short S16;

typedef unsigned int U32;
typedef signed int S32;

/* typedef unsigned long long U64;
typedef signed long long S64; */

typedef float F32;
typedef double F64;

typedef void U0;
typedef void* Ptr;

typedef struct { F64 x, y; } Vec2F64;

/* This must be mutable, but should not be altered! */
#ifndef __cplusplus
extern mut Ptr nullptr;
#undef NULL
#define NULL nullptr
#endif /* __cplusplus */

#endif /* UTIL_H */

