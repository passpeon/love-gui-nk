#include <lua.h>
#include <stdio.h>
#include <stdlib.h>

#include "draw.h"
#include "util.h"
#include "../libs/nuklear/nuklear.h"

extern lua_State *L;
extern struct nk_context context;

static void lgnk_draw_rect(short x, short y, short width, short height, unsigned short rounding, unsigned short line_thickness, struct nk_color color) {
    lua_getglobal(L, "love");
    lua_getfield(L, -1, "graphics");
    lua_remove(L, -2);

    lua_getfield(L, -1, "setLineWidth");
    lua_pushnumber(L, line_thickness);
    lua_call(L, 1, 0);

    lua_getfield(L, -1, "setColor");
    lua_pushnumber(L, color.r / 255.0);
    lua_pushnumber(L, color.g / 255.0);
    lua_pushnumber(L, color.b / 255.0);
    lua_pushnumber(L, color.a / 255.0);
    lua_call(L, 4, 0);

    lua_getfield(L, -1, "rectangle");
    lua_pushstring(L, "line");
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    lua_pushnumber(L, width);
    lua_pushnumber(L, height);
    lua_pushnumber(L, rounding);
    lua_pushnumber(L, rounding);
    lua_call(L, 7, 0);
    lua_pop(L, 1);
}

int lgnk_draw() {
    // Make sure our stack is empty
    lgnk_assert(lua_gettop(L) == 0, "Expected stack to be empty in lgnk_draw");

    const struct nk_command *cmd;
    nk_foreach(cmd, &context) {
        switch (cmd->type) {
            case NK_COMMAND_NOP:
                // Do nothing
                break;

            case NK_COMMAND_SCISSOR:
                break;
            case NK_COMMAND_LINE:
                break;
            case NK_COMMAND_CURVE:
                break;

            case NK_COMMAND_RECT: {
                const struct nk_command_rect *r = (const struct nk_command_rect *) cmd;
                lgnk_draw_rect(r->x, r->y, r->w, r->h, r->rounding, r->line_thickness, r->color);
                break;
            }

            // ToDo:
            case NK_COMMAND_RECT_FILLED: {
                const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *) cmd;
                lgnk_draw_rect(r->x, r->y, r->w, r->h, r->rounding, 1, r->color);
                break;
            }

            case NK_COMMAND_RECT_MULTI_COLOR:
                break;
            case NK_COMMAND_CIRCLE:
                break;
            case NK_COMMAND_CIRCLE_FILLED:
                break;
            case NK_COMMAND_ARC:
                break;
            case NK_COMMAND_ARC_FILLED:
                break;
            case NK_COMMAND_TRIANGLE:
                break;
            case NK_COMMAND_TRIANGLE_FILLED:
                break;
            case NK_COMMAND_POLYGON:
                break;
            case NK_COMMAND_POLYGON_FILLED:
                break;
            case NK_COMMAND_POLYLINE:
                break;
            case NK_COMMAND_TEXT:
                break;
            case NK_COMMAND_IMAGE:
                break;
            case NK_COMMAND_CUSTOM:
                break;
        }
    }

    lua_getfield(L, -1, "pop");
    lua_call(L, 0, 0);
    lua_pop(L, 2);
    nk_clear(&context);

    return 0;
}

