#ifndef MIAOUMOD_LUA_PCALL_HPP
#define MIAOUMOD_LUA_PCALL_HPP

namespace miaoumod{
namespace lua{

int pcall(lua_State * L, int nargs, int nresults, int error_function);

} //namespace lua
} //namespace miaoumod

#endif
