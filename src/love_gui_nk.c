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

// Internal
static lua_State *L;
static struct nk_context context;
struct nk_user_font *default_font;

/**
 * Print message to love console.
 */
static void lgnk_print(const char *msg) {
    lua_getglobal(L, "print");
    lua_pushstring(L, msg);
    lua_call(L, 1, 0);
}

/**
 * Prints current lua stack to love console.
 */
static void lgnk_dump_stack() {
    int i = lua_gettop(L);
    char *text;
    lgnk_print("--------------- Stack Dump Started ----------------");

    while (i) {
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:
                text = malloc((size_t) (snprintf(NULL, 0, "%d: %s", i, lua_tostring(L, i)) + 1));
                sprintf(text, "%d: %s", i, lua_tostring(L, i));
                lgnk_print(text);
                free(text);
                break;

            case LUA_TBOOLEAN:
                text = malloc((size_t) (snprintf(NULL, 0, "%d: %s", i, lua_toboolean(L, i) ? "true" : "false") + 1));
                sprintf(text, "%d: %s", i, lua_toboolean(L, i) ? "true" : "false");
                lgnk_print(text);
                free(text);
                break;

            case LUA_TNUMBER:
                text = malloc((size_t) (snprintf(NULL, 0, "%d: %g", i, lua_tonumber(L, i)) + 1));
                sprintf(text, "%d: %g", i, lua_tonumber(L, i));
                lgnk_print(text);
                free(text);
                break;

            default:
                text = malloc((size_t) (snprintf(NULL, 0, "%d: %s", i, lua_typename(L, t)) + 1));
                sprintf(text, "%d: %s", i, lua_typename(L, t));
                lgnk_print(text);
                free(text);
                break;
        }
        i--;
    }

    lgnk_print("--------------- Stack Dump Finished ---------------");
}

/**
 * Raise an lua error with the given message if pass equals false.
 *
 * @param pass
 * @param msg
 */
static void lgnk_assert(bool pass, const char *msg) {
    if (!pass) {
        lua_Debug ar;
        ar.name = NULL;

        if (lua_getstack(L, 0, &ar)) {
            lua_getinfo(L, "n", &ar);
        }

        if (ar.name == NULL) {
            ar.name = "?";
        }

        luaL_error(L, msg, ar.name);
    }
}

/**
 * Allocate the given amount of memory and assert whether it succeeded.
 *
 * @param size
 */
static void *lgnk_malloc(size_t size) {
    void *mem = malloc(size);
    lgnk_assert(mem != NULL, "Out of memory in %s");
    return mem;
}

/**
 * Pushes the default font onto the lua stack.
 *
 * @return Number of elements placed on stack
 */
static int lgnk_get_default_font() {
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

    return 1;
}

#undef REGISTER_LUA_FUNCTION