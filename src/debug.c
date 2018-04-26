#include <lua.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

extern lua_State *L;

/**
 * Print message to love console.
 */
void lgnk_print(const char *msg) {
    lua_getglobal(L, "print");
    lua_pushstring(L, msg);
    lua_call(L, 1, 0);
}

/**
 * Prints current lua stack to love console.
 */
void lgnk_dump_stack() {
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