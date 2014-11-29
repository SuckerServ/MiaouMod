#ifndef MIAOUMOD_LUA_REGISTER_CLASS_HPP
#define MIAOUMOD_LUA_REGISTER_CLASS_HPP

#include <luajit-2.0/lua.hpp>

namespace lua{

void register_class(lua_State *, const char *, const luaL_Reg *);

} //namespace lua

#endif

