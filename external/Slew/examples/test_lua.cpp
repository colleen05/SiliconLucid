// Project: Slew
// File: examples/test_lua.cpp
// Description: TEST example for lua.
// License: MIT
// https://github.com/colleen05/Slew

#include <slew/slew.hpp>
#include <lua.hpp>
#include <iostream>

int main() {
    slew::print_lib_info();
    std::cout << "\nLua test..." << std::endl;

    lua_State *L = luaL_newstate();

    luaL_dostring(L, "message = \"Hello, world!\"");
    luaL_dostring(L, "x = 45");

    lua_getglobal(L, "message");
    std::string message = lua_tostring(L, 1);
    std::cout << "message is \"" << message << "\"." << std::endl;
    lua_pop(L, 1);

    lua_getglobal(L, "x");
    int x = lua_tonumber(L, 1);
    std::cout << "x is " << x << "." << std::endl;
    lua_pop(L, 1);

    lua_close(L);

    return 0;
}