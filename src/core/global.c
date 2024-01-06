#ifndef GLOBAL_C
#define GLOBAL_C

#include "stdio.h"
#include "stdlib.h"
#include "util.h"
#include "simple_renderer.h"
#include "glfw3.h"

mut GLFWwindow* G_window;
mut sr_Renderer* G_rend;
mut F32 time_scale = 1.f;
mut mtar_t G_tar;
mut mtar_header_t G_tar_header;

#endif /* GLOBAL_C */

