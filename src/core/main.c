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

  mtar_open(&G_tar, (char*)"game.tar", (char*)"r");
  // mtar_read_header(&G_tar, &G_tar_header);

  mtar_find(&G_tar, (char*)"main.lua", &G_tar_header);
  mut char* main = calloc(1, G_tar_header.size + 1);
  mtar_read_data(&G_tar, main, G_tar_header.size);

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

  luaL_dostring(lua, main);

  sr_free(G_rend);
  free(G_rend);

  lua_settop(lua, 0);
  lua_close(lua);
  free(main);

  glfwDestroyWindow(G_window);
  glfwTerminate();

  mtar_close(&G_tar);

  return 0;
}

