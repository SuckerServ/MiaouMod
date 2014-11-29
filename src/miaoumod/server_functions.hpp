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
    int player_ownernum(int);
    std::string player_name(int);
    const char * player_privilege(int);
    int player_privilege_code(int);
    int player_ping(int);
    int player_ping_update(int);
    int player_lag(int);
    int player_real_lag(int);
    int player_maploaded(int);
    int player_deathmillis(int);
    const char * player_ip(int);
    unsigned long player_iplong(int);
    const char * player_status(int);
    int player_status_code(int);
    int player_frags(int);
    int player_score(int);
    int player_real_frags(int);
    int player_deaths(int);
    int player_suicides(int);
    int player_teamkills(int);
    int player_damage(int);
    int player_damagewasted(int);
    int player_maxhealth(int);
    int player_health(int);
    int player_gun(int);
    int player_hits(int);
    int player_misses(int);
    int player_shots(int);
    int player_accuracy(int);
    int player_accuracy2(int);
    int player_clientmillis(int);
    int player_timetrial(int);
    int player_connection_time(int);
    bool player_has_joined_game(int);
    void player_join_game(int);
    void player_reject_join_game(int);
    int player_timeplayed(int);
    void player_slay(int);

    void player_msg(int, const char *);
    void server_msg(const char *);

    void player_rename(int, const char *, bool);
} // namespace miaoumod

#endif
