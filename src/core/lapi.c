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
  lua_pushnumber(lua, get_fps());
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
  if (lua_toboolean(lua, 1)) glfwShowWindow(G_window);
  else glfwHideWindow(G_window);
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

static S32 lapi_get_chunk_tar(lua_State* lua) {
  let char* filename = lua_tostring(lua, 1);
  mut char* string_buf = nullptr;
  mtar_find(&G_tar, filename, &G_tar_header);
  string_buf = calloc(1, G_tar_header.size + 1);
  mtar_read_data(&G_tar, string_buf, G_tar_header.size);
  luaL_loadbuffer(lua, string_buf, G_tar_header.size, filename);
  free(string_buf);
  return 1;
}

let static luaL_Reg lib[] = {
  { "clear", lapi_clear },
  { "create_window", lapi_create_window },
  { "delay", lapi_delay },
  { "draw_rect", lapi_draw_rect },
  { "get_delta_time", lapi_get_delta_time },
  { "get_mouse_pos", lapi_get_mouse_pos },
  { "get_white_texture", lapi_get_white_tex },
  { "key_down", lapi_is_key_down },
  { "render_begin", lapi_render_begin },
  { "render_end", lapi_render_end },
  { "get_chunk_tar", lapi_get_chunk_tar },
  { "set_clear_color", lapi_clear_color },
  { "set_time_scale", lapi_set_time_scale },
  { "set_window_size", lapi_set_window_size },
  { "set_window_title", lapi_set_window_title },
  { "show_window", lapi_show_mouse },
  { "show_window", lapi_show_window },
  { "window_should_close", lapi_should_close },
  { NULL, NULL },
};

/* This is highly dumb... */
mut static char buf[512];
static U0 set_int(lua_State* lua, char* name, S32 v) {
  sprintf(buf, "snot.%s = %d", name, v);
  luaL_dostring(lua, buf);
}
#define KEYS \
  X("KEY_SPACE", 32) \
  X("KEY_APOSTROPHE", 39) \
  X("KEY_COMMA", 44) \
  X("KEY_MINUS", 45) \
  X("KEY_PERIOD", 46) \
  X("KEY_SLASH", 47) \
  X("KEY_0", 48) \
  X("KEY_1", 49) \
  X("KEY_2", 50) \
  X("KEY_3", 51) \
  X("KEY_4", 52) \
  X("KEY_5", 53) \
  X("KEY_6", 54) \
  X("KEY_7", 55) \
  X("KEY_8", 56) \
  X("KEY_9", 57) \
  X("KEY_SEMICOLON", 59) \
  X("KEY_EQUAL", 61) \
  X("KEY_A", 65) \
  X("KEY_B", 66) \
  X("KEY_C", 67) \
  X("KEY_D", 68) \
  X("KEY_E", 69) \
  X("KEY_F", 70) \
  X("KEY_G", 71) \
  X("KEY_H", 72) \
  X("KEY_I", 73) \
  X("KEY_J", 74) \
  X("KEY_K", 75) \
  X("KEY_L", 76) \
  X("KEY_M", 77) \
  X("KEY_N", 78) \
  X("KEY_O", 79) \
  X("KEY_P", 80) \
  X("KEY_Q", 81) \
  X("KEY_R", 82) \
  X("KEY_S", 83) \
  X("KEY_T", 84) \
  X("KEY_U", 85) \
  X("KEY_V", 86) \
  X("KEY_W", 87) \
  X("KEY_X", 88) \
  X("KEY_Y", 89) \
  X("KEY_Z", 90) \
  X("KEY_LEFT_BRACKET", 91) \
  X("KEY_BACKSLASH", 92) \
  X("KEY_RIGHT_BRACKET", 93) \
  X("KEY_GRAVE_ACCENT", 96) \
  X("KEY_WORLD_1", 161) \
  X("KEY_WORLD_2", 162) \
  X("KEY_ESCAPE", 256) \
  X("KEY_ENTER", 257) \
  X("KEY_TAB", 258) \
  X("KEY_BACKSPACE", 259) \
  X("KEY_INSERT", 260) \
  X("KEY_DELETE", 261) \
  X("KEY_RIGHT", 262) \
  X("KEY_LEFT", 263) \
  X("KEY_DOWN", 264) \
  X("KEY_UP", 265) \
  X("KEY_PAGE_UP", 266) \
  X("KEY_PAGE_DOWN", 267) \
  X("KEY_HOME", 268) \
  X("KEY_END", 269) \
  X("KEY_CAPS_LOCK", 280) \
  X("KEY_SCROLL_LOCK", 281) \
  X("KEY_NUM_LOCK", 282) \
  X("KEY_PRINT_SCREEN", 283) \
  X("KEY_PAUSE", 284) \
  X("KEY_F1", 290) \
  X("KEY_F2", 291) \
  X("KEY_F3", 292) \
  X("KEY_F4", 293) \
  X("KEY_F5", 294) \
  X("KEY_F6", 295) \
  X("KEY_F7", 296) \
  X("KEY_F8", 297) \
  X("KEY_F9", 298) \
  X("KEY_F10", 299) \
  X("KEY_F11", 300) \
  X("KEY_F12", 301) \
  X("KEY_F13", 302) \
  X("KEY_F14", 303) \
  X("KEY_F15", 304) \
  X("KEY_F16", 305) \
  X("KEY_F17", 306) \
  X("KEY_F18", 307) \
  X("KEY_F19", 308) \
  X("KEY_F20", 309) \
  X("KEY_F21", 310) \
  X("KEY_F22", 311) \
  X("KEY_F23", 312) \
  X("KEY_F24", 313) \
  X("KEY_F25", 314) \
  X("KEY_KP_0", 320) \
  X("KEY_KP_1", 321) \
  X("KEY_KP_2", 322) \
  X("KEY_KP_3", 323) \
  X("KEY_KP_4", 324) \
  X("KEY_KP_5", 325) \
  X("KEY_KP_6", 326) \
  X("KEY_KP_7", 327) \
  X("KEY_KP_8", 328) \
  X("KEY_KP_9", 329) \
  X("KEY_KP_DECIMAL", 330) \
  X("KEY_KP_DIVIDE", 331) \
  X("KEY_KP_MULTIPLY", 332) \
  X("KEY_KP_SUBTRACT", 333) \
  X("KEY_KP_ADD", 334) \
  X("KEY_KP_ENTER", 335) \
  X("KEY_KP_EQUAL", 336) \
  X("KEY_LEFT_SHIFT", 340) \
  X("KEY_LEFT_CONTROL", 341) \
  X("KEY_LEFT_ALT", 342) \
  X("KEY_LEFT_SUPER", 343) \
  X("KEY_RIGHT_SHIFT", 344) \
  X("KEY_RIGHT_CONTROL", 345) \
  X("KEY_RIGHT_ALT", 346) \
  X("KEY_RIGHT_SUPER", 347) \
  X("KEY_MENU", 348)

static inline U0 lapi_init(lua_State* lua) {
  luaL_newlib(lua, lib);
  lua_setglobal(lua, "snot");
#define X(n, v) set_int(lua, n, v);
  KEYS
#undef X
  luaL_dostring(lua, (char*)
    "snot.loaded_modules = {}\n"
    "require_tar = function(path)\n"
    "  if snot.loaded_modules.path == nil then\n"
    "    snot.loaded_modules.path = snot.get_chunk_tar(path .. '.lua')()\n"
    "  end\n"
    "  return snot.loaded_modules.path\n"
    "end"
  );
}
#undef KEYS

