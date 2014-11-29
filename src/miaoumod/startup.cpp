#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "miaoumod.hpp"
#include "events.hpp"
#include "utils/files.hpp"

extern void cleanupserver();
extern void kicknonlocalclients(int reason = 0);

namespace miaoumod {
void shutdown(int val)
{
    kicknonlocalclients();

    event_shutdown(event_listeners(), std::make_tuple(static_cast<int>(SHUTDOWN_NORMAL)));

    lua::shutdown(miaoumod::lua::get_lua_state());
    delete_temp_files_on_shutdown(val);

    cleanupserver();

    exit(0);
}


void register_signals(lua_State *L)
{
    struct sigaction terminate_action;
    sigemptyset(&terminate_action.sa_mask);
    terminate_action.sa_handler = miaoumod::shutdown;
    terminate_action.sa_flags = 0;
    sigaction(SIGINT, &terminate_action, NULL);
    sigaction(SIGTERM, &terminate_action, NULL);
}

/**
    Initializes everything in miaoumod. This function is called at server startup and server reload.
*/
void init()
{
    static const char * INIT_SCRIPT = "script/base/init.lua";

    temp_file_printf("log/tess_server.pid", "%i\n", getpid());

    lua_State * L = miaoumod::lua::init();

    register_signals(L);

    if(luaL_loadfile(L, INIT_SCRIPT) == 0)
    {
        miaoumod::event_listeners().add_listener("init"); // Take the value of the top of the stack to add
        // to the init listeners table
    }
    else
    {
        std::cerr<<"error during initialization: "<<lua_tostring(L, -1)<<std::endl;
        lua_pop(L, 1);
    }

    if(miaoumod::event_init(miaoumod::event_listeners(), std::make_tuple()))
	std::cerr<<"ERROR"<<std::endl;
}

}
