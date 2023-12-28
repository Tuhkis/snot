#ifndef TIME_C
#define TIME_C

#include "util.h"

#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "glfw3.h"

/* Linux specific headers. */
#ifdef OS_LINUX
#include "time.h"
#include "errno.h"
#endif /* OS_LINUX */

/* Windows specific header. */
#ifdef OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif /* OS_WINDOWS */

#include "global.c"

/* Clang will eat my internal organs if this isn't here. */
#ifdef OS_LINUX
#ifndef nanosleep
extern S32 nanosleep(const struct timespec* a, struct timespec* b);
#endif  /* nanosleep */
#endif /* OS_LINUX */

static inline U0 sleep_ms(F32 ms) {
#ifdef OS_LINUX
  if (ms < 0.f) {
    return;
  }
  mut struct timespec ts = default_initialiser;
  mut S32 res = 0;
  ts.tv_sec = ms * 0.001f;
  ts.tv_nsec = ((U32)ms % 1000) * 1000000;
  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);
#endif /* OS_LINUX */
#ifdef OS_WINDOWS
  /* timeBeginPeriod(1); */
  Sleep((U64)ms);
  /* timeEndPeriod(1); */
#endif /* OS_WINDOWS */
}

static mut F32 now, prev = 0.f;
static inline F32 get_fps(F32 fps) {
  now = glfwGetTime();
  F32 delta_time = (now - prev);
  prev = now;
  return delta_time * time_scale;
}

#endif /* TIME_C */

