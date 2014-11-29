//#include <cassert>
#include <exception>
#include <iostream>
#include "miaoumod.hpp"
#include "events.hpp"
#include "lua/modules.hpp"
#include "core_bindings.hpp"

namespace miaoumod {
static miaoumod::lua::event_environment * event_environment = NULL;
static lua_State * Lg = NULL;
static int lua_stack_size = 0;

lua::event_environment & event_listeners()
{
    static lua::event_environment unready_event_environment;
    if(!miaoumod::event_environment) return unready_event_environment;
    lua_stack_size = lua_gettop(Lg);
    return *miaoumod::event_environment;
}

namespace lua {

     #include <execinfo.h>
     #include <stdio.h>
int panic(lua_State* L)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "debug");
    lua_getfield(L, -1, "traceback");
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 2);
    lua_call(L, 2, 1);
    fprintf(stderr, "%s\n", lua_tostring(L, -1));

    void* callstack[128];
     int i, frames = backtrace(callstack, 128);
     char** strs = backtrace_symbols(callstack, frames);
     for (i = 0; i < frames; ++i) {
         printf("%s\n", strs[i]);
     }
     free(strs);

    return 1;
}

lua_State * get_lua_state()
{
    return Lg;
}

int get_lua_stack_size()
{
    return miaoumod::lua_stack_size;
}

void log_event_error(const char * event_id, const char * error_message)
{
    event_listeners().log_error(event_id, error_message);
}

static void load_modules(lua_State * L)
{
    lua::module::open_crypto(L);
    lua::module::open_mmdb(L);
    lua::module::open_filesystem(L);
}

lua_State * init()
{
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    uv_loop_t *loop = uv_default_loop();

#ifdef USE_OPENSSL
//    luvit_init_ssl();
#endif

    if (luvit_init(L, loop))
        std::cerr<<"luvit_init has failed"<<std::endl;

#ifndef NO_CORE_TABLE
    lua_newtable(L);
    int core_table = lua_gettop(L);

    bind_core_functions(L, core_table);

    lua_pushliteral(L, "vars");
    lua_newtable(L);
    int vars_table = lua_gettop(L);

    bind_core_constants(L, vars_table);
    bind_core_variables(L, vars_table);

    lua_settable(L, -3); // Add vars table to core table
    lua_setglobal(L, "core"); // Add core table to global table
#endif

    miaoumod::event_environment = new lua::event_environment(L, NULL);

#ifndef NO_EVENTS
    register_event_idents(*miaoumod::event_environment); // Setup and populate the event table
#endif

    load_modules(L);

//    lua_atpanic(L, panic);

    miaoumod::Lg = L;

    return L;
}

void shutdown(lua_State * L)
{
    if(!L) return;

    uv_stop(uv_default_loop());

    lua_close(L);
}

}//namespace lua
}//namespace miaoumod
