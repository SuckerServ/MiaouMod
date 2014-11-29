#include <iostream>
#include <unistd.h>
#include "miaoumod.hpp"
#include "events.hpp"
#include "utils/files.hpp"

namespace miaoumod {

/**
    Initializes everything in miaoumod. This function is called at server startup and server reload.
*/
void init()
{
    static const char * INIT_SCRIPT = "script/base/init.lua";

    temp_file_printf("log/tess_server.pid", "%i\n", getpid());

    lua_State * L = miaoumod::lua::init();

/*    if(luaL_loadfile(L, INIT_SCRIPT) == 0)
    {
        miaoumod::event_listeners().add_listener("init"); // Take the value of the top of the stack to add
        // to the init listeners table
    }
    else
    {
        std::cerr<<"error during initialization: "<<lua_tostring(L, -1)<<std::endl;
        lua_pop(L, 1);
    }

    miaoumod::event_init(miaoumod::event_listeners(), std::make_tuple());*/

 luaL_dostring(L, "package.path = package.path .. \";script/base/?.lua\"");
if(luaL_dostring(L, "require \"init\""))
{
printf("%s\n", lua_tostring(L, -1));
lua_pop(L, 1);
lua_close(L);
}

}

void shutdown(int val, lua_State * L)
{
    lua::shutdown(L);
    delete_temp_files_on_shutdown(val);
}

}
