#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#define NK_IMPLEMENTATION
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_PRIVATE

#include "../libs/nuklear/nuklear.h"

#include "debug.h"
#include "util.h"
#include "draw.h"

// Internal
lua_State *L;
struct nk_context context;
struct nk_user_font *default_font;

/**
 * Pushes the default font onto the lua stack.
 *
 * @return Number of elements placed on stack
 */
static int lgnk_get_default_font() {
    // ToDo: Remove fields from stack? Keep only font?

    lua_getglobal(L, "love");
    lua_getfield(L, -1, "graphics");
    lua_getfield(L, -1, "getFont");
    lua_call(L, 0, 1);

    return 3;
}

// ToDo: Multiple fonts
static float lgnk_get_font_width(nk_handle handle, float height, const char *text, int len) {
    int stack = lgnk_get_default_font();

    // Get width
    lua_getfield(L, -1, "getWidth");
    lua_pushvalue(L, -2);
    lua_pushlstring(L, text, (size_t) len);
    lua_call(L, 2, 1);

    float width = (float) lua_tonumber(L, -1);

    lua_pop(L, stack + 4);

    return width;
}

// ToDo: Multiple fonts
static float lgnk_get_font_height() {
    int stack = lgnk_get_default_font();

    // Get height
    lua_getfield(L, -1, "getHeight");
    lua_pushvalue(L, -2);
    lua_call(L, 1, 1);

    float height = (float) lua_tonumber(L, -1);

    lua_pop(L, stack + 1);

    return height;
}

static int lgnk_init(lua_State *luaState) {
    L = luaState;

    // Setup default font
    default_font = lgnk_malloc(sizeof(struct nk_user_font));

    default_font->userdata = nk_handle_id(1); // ToDo
    default_font->height = lgnk_get_font_height();
    default_font->width = lgnk_get_font_width;

    lgnk_dump_stack();

    // Init
    nk_init_default(&context, default_font);

    nk_begin_titled(&context, "Test", "Test", nk_rect(10, 10, 200, 200), NK_WINDOW_BORDER);
    nk_end(&context);

    return 0;
}

static int lgnk_shutdown(lua_State *luaState) {
    nk_free(&context);
    free(default_font);
    L = NULL;

    return 0;
}

// Register LUA functions
#define REGISTER_LUA_FUNCTION(name, func) \
    lua_pushcfunction(L, func); \
    lua_setfield(L, -2, name)

LUALIB_API int luaopen_nuklear(lua_State *L) {
    lua_newtable(L);

    REGISTER_LUA_FUNCTION("init", lgnk_init);
    REGISTER_LUA_FUNCTION("shutdown", lgnk_shutdown);
    REGISTER_LUA_FUNCTION("draw", lgnk_draw);

    return 1;
}

#undef REGISTER_LUA_FUNCTION