/* #include "../../external/ini/src/ini.c" */

#include "stdio.h"
#include "stdlib.h"
#include "util.h"
#include "simple_renderer.h"
#include "glfw3.h"
#include "minilua.h"
#include "microtar.h"

/**
 * Headers be damned am I right???
 */
#include "global.c"
#include "lapi.c"

mut Ptr nullptr = (Ptr)0;

/* TODO: read https://www.oreilly.com/library/view/creating-solid-apis/9781491986301/ch01.html */

MAIN {
  mut lua_State* lua;
  /*mut mtar_t tar;
  mut mtar_header_t tar_header;

  mtar_open(&tar, "game.tar", "r");
  mtar_read_header(&tar, &tar_header); */

  ASSERT(glfwInit());
  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  lua = luaL_newstate();
  ASSERT(lua != nullptr);
  luaL_openlibs(lua);
  lapi_init(lua);

  lua_settop(lua, 0);

  luaL_dofile(lua, "main.lua");

  sr_free(G_rend);
  free(G_rend);

  lua_settop(lua, 0);
  lua_close(lua);
  
  glfwDestroyWindow(G_window);
  glfwTerminate();

  /* mtar_close(&tar); */
  
  return 0;
}

