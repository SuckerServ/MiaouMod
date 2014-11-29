#ifndef MIAOUMOD_BIND_FUNCS_HPP
#define MIAOUMOD_BIND_FUNCS_HPP

#include "lua.hpp"
#include "cube.h"

/* Forward declaration of Lua value io functions */
//#include "lua/push_function_fwd.hpp"

#include "lua/push_function.hpp"
#include <iostream>
#include <vector>
#include <string>

namespace miaoumod{
namespace lua{
//void push(lua_State * L, string value);
//void push(lua_State * L, __uid_t value);

/*
    Lua value io functions for cube2 types
*/
//void push(lua_State * L, string value)
//{
//    lua_pushstring(L, value);
//}

template<typename FunctionPointerType>
static void bind_function(lua_State * L, int table, const char * name, FunctionPointerType function)
{
    lua_pushstring(L, name);
    lua::push_function(L, function);
    lua_settable(L, table);
}

static void bind_function(lua_State * L, int table, const char * name, lua_CFunction function)
{
    lua_pushstring(L, name);
    lua_pushcfunction(L, function);
    lua_settable(L, table);
}

template<int Constant>
static int get_constant(lua_State * L)
{
    if(lua_gettop(L)) luaL_error(L, "cannot set constant");
    lua_pushinteger(L, Constant);
    return 1;
}

template<typename T, bool READ_ONLY, bool WRITE_ONLY>
static int variable_accessor(lua_State * L)
{
    T * var = reinterpret_cast<T *>(lua_touserdata(L, lua_upvalueindex(1)));
    if(lua_gettop(L) > 0) // Set variable
    {
        if(READ_ONLY) luaL_error(L, "variable is read-only");
        *var = lua::to(L, 1, lua::return_tag<T>());
//        event_varchanged(event_listeners(), std::make_tuple(lua_tostring(L, lua_upvalueindex(2))));
        return 0;
    }
    else // Get variable
    {
        if(WRITE_ONLY) luaL_error(L, "variable is write-only");
        miaoumod::lua::push(L, *var);
        return 1;
    }
}

template<bool READ_ONLY, bool WRITE_ONLY>
static int string_accessor(lua_State * L)
{
    char * var = reinterpret_cast<char *>(lua_touserdata(L, lua_upvalueindex(1)));
    if(lua_gettop(L) > 0) // Set variable
    {
        if(READ_ONLY) luaL_error(L, "variable is read-only");
        convert2cube varcubeenc(lua_tostring(L, 1));
        copystring(var, varcubeenc.str());
//        event_varchanged(event_listeners(), std::make_tuple(lua_tostring(L, lua_upvalueindex(2))));
        return 0;
    }
    else // Get variable
    {
        if(WRITE_ONLY) luaL_error(L, "variable is write-only");
        convert2utf8 varutf8(var);
        miaoumod::lua::push(L, varutf8.str());
        return 1;
    }
}

template<typename T>
static void bind_var(lua_State * L, int table, const char * name, T & var)
{
    lua_pushstring(L, name);
    lua_pushlightuserdata(L, &var);
    lua_pushstring(L, name);
    lua_pushcclosure(L, variable_accessor<T, false, false>, 2);
    lua_settable(L, table);
}

static void bind_var(lua_State * L, int table, const char * name, string var)
{
    lua_pushstring(L, name);
    lua_pushlightuserdata(L, var);
    lua_pushstring(L, name);
    lua_pushcclosure(L, string_accessor<false, false>, 2);
    lua_settable(L, table);
}

template<typename T>
static void bind_ro_var(lua_State * L, int table, const char * name, T & var)
{
    lua_pushstring(L, name);
    lua_pushlightuserdata(L, &var);
    lua_pushstring(L, name);
    lua_pushcclosure(L, variable_accessor<T, true, false>, 2);
    lua_settable(L, table);
}

static void bind_ro_var(lua_State * L, int table, const char * name, string var)
{
    lua_pushstring(L, name);
    lua_pushlightuserdata(L, var);
    lua_pushstring(L, name);
    lua_pushcclosure(L, string_accessor<true, false>, 2);
    lua_settable(L, table);
}

template<typename T>
static void bind_wo_var(lua_State * L, int table, const char * name, T & var)
{
    lua_pushstring(L, name);
    lua_pushlightuserdata(L, &var);
    lua_pushstring(L, name);
    lua_pushcclosure(L, variable_accessor<T, false, true>, 2);
    lua_settable(L, table);
}

static void bind_wo_var(lua_State * L, int table, const char * name, string var)
{
    lua_pushstring(L, name);
    lua_pushlightuserdata(L, var);
    lua_pushstring(L, name);
    lua_pushcclosure(L, string_accessor<false, true>, 2);
    lua_settable(L, table);
}

template<typename T>
static int property_accessor(lua_State * L)
{
    T (* get)() = reinterpret_cast<T (*)()>(lua_touserdata(L, lua_upvalueindex(1)));
    void (* set)(T) = reinterpret_cast<void (*)(T)>(lua_touserdata(L, lua_upvalueindex(2)));
    if(lua_gettop(L) > 0)
    {
        if(!set) luaL_error(L, "cannot set value");
        set(lua::to(L, 1, lua::return_tag<T>()));
//        event_varchanged(event_listeners(), std::make_tuple(lua_tostring(L, lua_upvalueindex(3))));
        return 0;
    }
    else
    {
        if(!get) luaL_error(L, "cannot get value");
        miaoumod::lua::push(L, get());
        return 1;
    }
}

template<typename T>
static void bind_prop(lua_State * L, int table, const char * name, T (* get)(), void (* set)(T))
{
    lua_pushstring(L, name);
    lua_pushlightuserdata(L, reinterpret_cast<void *>(get));
    lua_pushlightuserdata(L, reinterpret_cast<void *>(set));
    lua_pushstring(L, name);
    lua_pushcclosure(L, property_accessor<T>, 3);
    lua_settable(L, table);
}

}// lua
}// miaoumod

#endif
