#include <stdbool.h>

#ifndef LOVE_GUI_NK_UTIL_H
#define LOVE_GUI_NK_UTIL_H

void lgnk_assert(bool pass, const char *msg);

void *lgnk_malloc(size_t size);

#endif //LOVE_GUI_NK_UTIL_H
