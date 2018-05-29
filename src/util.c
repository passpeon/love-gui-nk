#include <stdbool.h>
#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>

#include "util.h"

extern lua_State *L;


/**
 * Raise an lua error with the given message if pass equals false.
 *
 * @param pass
 * @param msg
 */
void lgnk_assert(bool pass, const char *msg) {
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
void *lgnk_malloc(size_t size) {
    void *mem = malloc(size);
    lgnk_assert(mem != NULL, "Out of memory in %s");

    return mem;
}