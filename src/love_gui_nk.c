#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

static lua_State *L;

static int lgnk_init(lua_State *luaState) {
    L = luaState;

    lua_getglobal(L, "print");
    lua_pushstring(L, "It Works: Init");
    lua_call(L, 1, 0);

    return 0;
}

static int lgnk_shutdown(lua_State *luaState) {
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