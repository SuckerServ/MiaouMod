#ifndef MIAOUMOD_SERVER_FUNCTIONS_HPP
#define MIAOUMOD_SERVER_FUNCTIONS_HPP

namespace miaoumod{
    extern int intermtime;
    extern unsigned int packetdelay;

    int lua_player_list(lua_State *);
    int lua_spec_list(lua_State *);
    int lua_bot_list(lua_State *);
    int lua_client_list(lua_State *);

    int player_sessionid(int);
    int player_id(lua_State * L);
    const char * player_ip(int);

    void player_msg(int, const char *);
    void server_msg(const char *);

    void player_rename(int, const char *, bool);
} // namespace miaoumod

#endif
