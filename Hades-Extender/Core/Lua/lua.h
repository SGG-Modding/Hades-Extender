#pragma once
#define GLOBAL_TABLE "Extender"

/* Include all lua 5.2.2 dependencies */

#include <lua/lua.hpp>

extern "C" {
# include <lua/lua.h>
# include <lua/lauxlib.h>
# include <lua/lualib.h>
}

/* Link lua static library */
#pragma comment(lib, "liblua.lib")

/* Lua classes */
#include "./luafunction.h"
#include "./luawrapper.h"

/* Include our actual bindings */
#include "./bindings.h"