#include <iostream>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#pragma comment (lib, "lua5.1.lib")

using namespace std;

int main()
{
    lua_State* L = lua_open();
    luaL_openlibs(L);

    while (1)
    {
        int res = luaL_dofile(L, "../Script/func.lua");

        lua_getglobal(L, "LuaAdd");
        lua_pushnumber(L, 30);
        lua_pushnumber(L, 100);

        lua_call(L, 2, 2);
        auto num = lua_tointeger(L, lua_gettop(L));
        lua_pop(L, 1);
        cout << num << endl;

        num = lua_tointeger(L, lua_gettop(L));
        cout << num << endl;
    }
    

    lua_close(L);
}