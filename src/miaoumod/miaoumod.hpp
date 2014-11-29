#ifndef MIAOUMOD_HPP
#define MIAOUMOD_HPP

#include <luajit-2.0/lua.hpp>

#ifndef WITHOUT_OPENSSL
#define USE_OPENSSL
#endif

extern "C"
{
    #include "luvit/uv/uv.h"
    #include "luvit/luvit_init.h"
}

#include "lua/event.hpp"
#include "events.hpp"
#include "server_functions.hpp"

namespace miaoumod {
    void init();
    void shutdown(int);
    miaoumod::lua::event_environment & event_listeners();

    namespace lua {
        lua_State * init();
        lua_State * get_lua_state();
        void shutdown(lua_State *);
    }

    enum{
        SHUTDOWN_NORMAL,
        SHUTDOWN_RESTART,
        SHUTDOWN_RELOAD
    };
}

#endif
