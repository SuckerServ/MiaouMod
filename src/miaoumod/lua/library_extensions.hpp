#ifndef MIAOUMOD_LUA_LIBRARY_EXTENSIONS_HPP
#define MIAOUMOD_LUA_LIBRARY_EXTENSIONS_HPP

namespace lua{
    int getcwd(lua_State *L);
    int mkfifo(lua_State * L);
    int open_fifo(lua_State * L);
    int usleep(lua_State * L);
} //namespace lua

#endif

