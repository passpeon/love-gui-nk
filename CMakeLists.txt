CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT("love-gui-nk" C)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
FIND_PACKAGE(LuaJIT REQUIRED)

ADD_LIBRARY(
        "nuklear"
        MODULE
        libs/nuklear/nuklear.h
        src/main.c
        src/debug.c src/debug.h src/util.c src/util.h)

TARGET_INCLUDE_DIRECTORIES(
        "nuklear"
        PUBLIC
        "${LUA_INCLUDE_DIR}"
)

TARGET_LINK_LIBRARIES(
        "nuklear"
        ${LUA_LIBRARIES}
)

SET_TARGET_PROPERTIES("nuklear" PROPERTIES PREFIX "")