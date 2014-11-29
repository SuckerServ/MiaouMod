#include "bind_funcs.hpp"
#include "miaoumod.hpp"

using namespace miaoumod::lua;

void bind_core_functions(lua_State * L, int T)
{
    bind_function(L, T, "players", miaoumod::lua_player_list);
    bind_function(L, T, "spectators", miaoumod::lua_spec_list);
    bind_function(L, T, "bots", miaoumod::lua_bot_list);
    bind_function(L, T, "clients", miaoumod::lua_client_list);

    bind_function(L, T, "player_id", miaoumod::player_id);
    bind_function(L, T, "player_sessionid", miaoumod::player_sessionid);
    bind_function(L, T, "player_ip", miaoumod::player_ip);

    bind_function(L, T, "player_msg", miaoumod::player_msg);

    bind_function(L, T, "msg", miaoumod::server_msg);

}

void bind_core_constants(lua_State * L, int T)
{
}

void bind_core_variables(lua_State * L, int T)
{
    bind_var(L, T, "maxclients", maxclients);

    bind_var(L, T, "intermission_time", miaoumod::intermtime);
    bind_var(L, T, "packet_delay", miaoumod::packetdelay);

/*    bind_ro_var(L, T, "paused", server::gamepaused);
    bind_var(L, T, "gamespeed", server::gamespeed);
    bind_var(L, T, "servername", server::serverdesc);
    bind_ro_var(L, T, "map", server::smapname);
    bind_var(L, T, "server_password", server::serverpass);
    bind_var(L, T, "server_auth_domain", server::serverauth);
    bind_prop(L, T, "timeleft", server::get_minutes_left, server::set_minutes_left);
    bind_prop(L, T, "seconds_left", server::get_seconds_left, server::set_seconds_left);
    bind_prop<const char *>(L, T, "gamemode", server::gamemodename, NULL);
    bind_wo_var(L, T, "reserved_slots_password", server::slotpass);
*/
}

