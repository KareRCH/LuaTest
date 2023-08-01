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

    int res = luaL_dofile(L, "../Script/func.lua");

    lua_getglobal(L, "LuaAdd");
    lua_pushnumber(L, 30);
    lua_pushnumber(L, 100);
    lua_call(L, 2, 1);

    int num = lua_tointeger(L, lua_gettop(L));

    cout << num << endl;

    lua_close(L);
}