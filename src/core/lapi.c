#include "stdio.h"
#include "stdlib.h"
#include "util.h"
#include "simple_renderer.h"
#include "glfw3.h"
#include "minilua.h"

#include "time.c"
#include "global.c"

static S32 lapi_clear_color(lua_State* lua) {
  let F64 r = lua_tonumber(lua, 1);
  let F64 g = lua_tonumber(lua, 2);
  let F64 b = lua_tonumber(lua, 3);
  glClearColor((F32)r, (F32)g, (F32)b, 1.f);
  return 0;
}

static S32 lapi_clear(lua_State* lua) {
  glClear(GL_COLOR_BUFFER_BIT);
  return 0;
}

static S32 lapi_render_begin(lua_State* lua) {
  glfwPollEvents();
  sr_render_begin(G_rend);
  return 0;
}

static S32 lapi_render_end(lua_State* lua) {
  sr_render_end(G_rend);
  glfwSwapBuffers(G_window);
  return 0;
}

static S32 lapi_should_close(lua_State* lua) {
  lua_pushboolean(lua, glfwWindowShouldClose(G_window));
  return 1;
}

static S32 lapi_get_delta_time(lua_State* lua) {
  let F32 fps = lua_tonumber(lua, 1);
  lua_pushnumber(lua, get_fps(fps));
  return 1;
}

static S32 lapi_set_time_scale(lua_State* lua) {
  time_scale = lua_tonumber(lua, 1);
  return 0;
}

static S32 lapi_draw_rect(lua_State* lua) {
  sr_render_push_quad(G_rend,
    sr_vec2(lua_tonumber(lua, 1), lua_tonumber(lua, 2)),
    sr_vec2(lua_tonumber(lua, 3), lua_tonumber(lua, 4)),
    sr_vec4(lua_tonumber(lua, 5), lua_tonumber(lua, 6), lua_tonumber(lua, 7), lua_tonumber(lua, 8)), lua_tonumber(lua, 9)),
    lua_tointeger(lua, 10);
  return 0;
}

static S32 lapi_get_white_tex(lua_State* lua) {
  lua_pushinteger(lua, sr_get_white_texture());
  return 1;
}

static S32 lapi_set_window_size(lua_State* lua) {
  let F32 w = lua_tonumber(lua, 1);
  let F32 h = lua_tonumber(lua, 2);
  glfwSetWindowSize(G_window, w, h);
  sr_resize(G_rend, w, h);
  return 0;
}

static S32 lapi_set_window_title(lua_State* lua) {
  glfwSetWindowTitle(G_window, lua_tostring(lua, 1));
  return 0;
}

static S32 lapi_show_window(lua_State* lua) {
  glfwShowWindow(G_window);
  return 0;
}

static S32 lapi_delay(lua_State* lua) {
  sleep_ms(lua_tonumber(lua, 1));
  return 0;
}

static S32 lapi_create_window(lua_State* lua) {
  let F32 w = lua_tonumber(lua, 1);
  let F32 h = lua_tonumber(lua, 2);
  let F32 scale = lua_tonumber(lua, 3);
  let char* title = lua_tostring(lua, 4);

  mut S32 mon_pos_x;
  mut S32 mon_pos_y;

  GLFWmonitor* mon = glfwGetPrimaryMonitor();
  let GLFWvidmode* video = glfwGetVideoMode(mon);
  glfwGetMonitorPos(mon, &mon_pos_x, &mon_pos_y);

  G_window = glfwCreateWindow(w, h, title, nullptr, nullptr);
  glfwSetWindowPos(G_window,
    mon_pos_x + (.5 * video->width -  .5 * w), mon_pos_y + (.5 * video->height - .5 * h));
  ASSERT(G_window != nullptr);
  glfwMakeContextCurrent(G_window);
  glfwSwapInterval(lua_toboolean(lua, 5));
  sr_load_loader((Ptr)glfwGetProcAddress);
  glClearColor(.4f, .653f, 1.f, 1.f);
  G_rend = malloc(sizeof(sr_Renderer));
  sr_init(G_rend, w * scale, h * scale);
  glfwShowWindow(G_window);

  return 0;
}

static S32 lapi_is_key_down(lua_State* lua) {
  lua_pushboolean(lua, glfwGetKey(G_window, lua_tointeger(lua, 1)) == GLFW_PRESS);
  return 1;
}

static S32 lapi_get_mouse_pos(lua_State* lua) {
  mut F64 x;
  mut F64 y;
  glfwGetCursorPos(G_window, &x, &y);
  lua_pushnumber(lua, x);
  lua_pushnumber(lua, y);
  return 2;
}

static S32 lapi_show_mouse(lua_State* lua) {
  if (lua_toboolean(lua, 1))
    glfwSetInputMode(G_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  else
    glfwSetInputMode(G_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  return 0;
}

static inline void lapi_init(lua_State* lua) {
  lua_pushcfunction(lua, lapi_clear_color);
  lua_setglobal(lua, "c_clear_color");

  lua_pushcfunction(lua, lapi_clear);
  lua_setglobal(lua, "c_clear");

  lua_pushcfunction(lua, lapi_render_begin);
  lua_setglobal(lua, "c_render_begin");

  lua_pushcfunction(lua, lapi_render_end);
  lua_setglobal(lua, "c_render_end");

  lua_pushcfunction(lua, lapi_should_close);
  lua_setglobal(lua, "c_should_close");

  lua_pushcfunction(lua, lapi_get_delta_time);
  lua_setglobal(lua, "c_get_delta_time");

  lua_pushcfunction(lua, lapi_set_time_scale);
  lua_setglobal(lua, "c_set_time_scale");

  lua_pushcfunction(lua, lapi_draw_rect);
  lua_setglobal(lua, "c_draw_rect");

  lua_pushcfunction(lua, lapi_get_white_tex);
  lua_setglobal(lua, "c_get_white_texture");

  lua_pushcfunction(lua, lapi_set_window_size);
  lua_setglobal(lua, "c_set_window_size");

  lua_pushcfunction(lua, lapi_set_window_title);
  lua_setglobal(lua, "c_set_window_title");

  lua_pushcfunction(lua, lapi_show_window);
  lua_setglobal(lua, "c_show_window");

  lua_pushcfunction(lua, lapi_create_window);
  lua_setglobal(lua, "c_create_window");

  lua_pushcfunction(lua, lapi_is_key_down);
  lua_setglobal(lua, "c_is_key_down");

  lua_pushcfunction(lua, lapi_delay);
  lua_setglobal(lua, "c_delay");

  lua_pushcfunction(lua, lapi_get_mouse_pos);
  lua_setglobal(lua, "c_get_mouse_pos");

  lua_pushcfunction(lua, lapi_show_mouse);
  lua_setglobal(lua, "c_show_mouse");
}

