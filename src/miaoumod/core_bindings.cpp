#include "bind_funcs.hpp"
#include "miaoumod.hpp"
#include "game.h"

using namespace miaoumod::lua;

void bind_core_functions(lua_State * L, int T)
{
    bind_function(L, T, "shutdown", miaoumod::shutdown);

    bind_function(L, T, "players", miaoumod::lua_player_list);
    bind_function(L, T, "spectators", miaoumod::lua_spec_list);
    bind_function(L, T, "bots", miaoumod::lua_bot_list);
    bind_function(L, T, "clients", miaoumod::lua_client_list);

    bind_function(L, T, "player_id", miaoumod::player_id);
    bind_function(L, T, "player_sessionid", miaoumod::player_sessionid);
    bind_function(L, T, "player_ip", miaoumod::player_ip);
    bind_function(L, T, "player_name", miaoumod::player_name);
    bind_function(L, T, "player_rename", miaoumod::player_rename);
    bind_function(L, T, "player_priv", miaoumod::player_privilege);
    bind_function(L, T, "player_priv_code", miaoumod::player_privilege_code);
    bind_function(L, T, "player_sessionid", miaoumod::player_sessionid);
    bind_function(L, T, "player_ping", miaoumod::player_ping);
    bind_function(L, T, "player_ping_update", miaoumod::player_ping_update);
    bind_function(L, T, "player_lag", miaoumod::player_lag);
    bind_function(L, T, "player_real_lag", miaoumod::player_real_lag);
    bind_function(L, T, "player_maploaded", miaoumod::player_maploaded);
    bind_function(L, T, "player_deathmillis", miaoumod::player_deathmillis);
    bind_function(L, T, "player_iplong", miaoumod::player_iplong);
    bind_function(L, T, "player_status", miaoumod::player_status);
    bind_function(L, T, "player_status_code", miaoumod::player_status_code);
    bind_function(L, T, "player_frags", miaoumod::player_frags);
    bind_function(L, T, "player_score", miaoumod::player_score);
    bind_function(L, T, "player_deaths", miaoumod::player_deaths);
    bind_function(L, T, "player_suicides", miaoumod::player_suicides);
    bind_function(L, T, "player_teamkills", miaoumod::player_teamkills);
    bind_function(L, T, "player_damage", miaoumod::player_damage);
    bind_function(L, T, "player_damagewasted", miaoumod::player_damagewasted);
    bind_function(L, T, "player_maxhealth", miaoumod::player_maxhealth);
    bind_function(L, T, "player_health", miaoumod::player_health);
    bind_function(L, T, "player_gun", miaoumod::player_gun);
    bind_function(L, T, "player_hits", miaoumod::player_hits);
    bind_function(L, T, "player_misses", miaoumod::player_misses);
    bind_function(L, T, "player_shots", miaoumod::player_shots);
    bind_function(L, T, "player_accuracy", miaoumod::player_accuracy);
    bind_function(L, T, "player_accuracy2", miaoumod::player_accuracy2);
    bind_function(L, T, "player_clientmillis", miaoumod::player_clientmillis);
    bind_function(L, T, "player_timeplayed", miaoumod::player_timeplayed);
    bind_function(L, T, "player_slay", miaoumod::player_slay);

    bind_function(L, T, "player_msg", miaoumod::player_msg);

    bind_function(L, T, "msg", miaoumod::server_msg);
}

void bind_core_constants(lua_State * L, int T)
{
    bind_function(L, T, "DISC_NONE", get_constant<DISC_NONE>);
    bind_function(L, T, "DISC_EOP", get_constant<DISC_EOP>);
    bind_function(L, T, "DISC_LOCAL", get_constant<DISC_LOCAL>);
    bind_function(L, T, "DISC_KICK", get_constant<DISC_KICK>);
    bind_function(L, T, "DISC_MSGERR", get_constant<DISC_MSGERR>);
    bind_function(L, T, "DISC_IPBAN", get_constant<DISC_IPBAN>);
    bind_function(L, T, "DISC_PRIVATE", get_constant<DISC_PRIVATE>);
    bind_function(L, T, "DISC_MAXCLIENTS", get_constant<DISC_MAXCLIENTS>);
    bind_function(L, T, "DISC_TIMEOUT", get_constant<DISC_TIMEOUT>);
    bind_function(L, T, "DISC_OVERFLOW", get_constant<DISC_OVERFLOW>);
    bind_function(L, T, "DISC_NUM", get_constant<DISC_NUM>);

    bind_function(L, T, "PRIV_NONE", get_constant<PRIV_NONE>);
    bind_function(L, T, "PRIV_MASTER", get_constant<PRIV_MASTER>);
    bind_function(L, T, "PRIV_AUTH", get_constant<PRIV_AUTH>);
    bind_function(L, T, "PRIV_ADMIN", get_constant<PRIV_ADMIN>);

    bind_function(L, T, "ALIVE", get_constant<CS_ALIVE>);
    bind_function(L, T, "DEAD", get_constant<CS_DEAD>);
    bind_function(L, T, "SPAWNING", get_constant<CS_SPAWNING>);
    bind_function(L, T, "LAGGED", get_constant<CS_LAGGED>);
    bind_function(L, T, "SPECTATOR", get_constant<CS_SPECTATOR>);
    bind_function(L, T, "EDITING", get_constant<CS_EDITING>);

    bind_function(L, T, "MM_OPEN", get_constant<MM_OPEN>);
    bind_function(L, T, "MM_VETO", get_constant<MM_VETO>);
    bind_function(L, T, "MM_LOCKED", get_constant<MM_LOCKED>);
    bind_function(L, T, "MM_PRIVATE", get_constant<MM_PRIVATE>);
    bind_function(L, T, "MM_PASSWORD", get_constant<MM_PASSWORD>);
}

void bind_core_variables(lua_State * L, int T)
{
    bind_var(L, T, "maxclients", maxclients);

    bind_var(L, T, "intermission_time", miaoumod::intermtime);
    bind_var(L, T, "packet_delay", miaoumod::packetdelay);
}
