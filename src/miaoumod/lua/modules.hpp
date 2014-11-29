#ifndef MIAOUMOD_LUA_MODULES_HPP
#define MIAOUMOD_LUA_MODULES_HPP

namespace miaoumod{
namespace lua{
namespace module{

int open_crypto(lua_State *L);
int open_mmdb(lua_State *L);
int open_filesystem(lua_State *L);

} //namespace module
} //namespace lua
} //namespace miaoumod

#endif

