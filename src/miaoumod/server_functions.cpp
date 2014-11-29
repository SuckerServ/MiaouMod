using namespace server;

namespace miaoumod{

int intermtime = 10000;
unsigned int packetdelay = 40;

clientinfo * get_ci(int cn)
{
    clientinfo * ci = getinfo(cn);
    if(!ci) luaL_error(lua::get_lua_state(), "invalid cn");
    return ci;
}


int make_client_list(lua_State * L,bool (* clienttype)(clientinfo *))
{
    lua_newtable(L);
    int count = 0;

    loopv(clients) if(clienttype(clients[i]))
    {
        lua_pushinteger(L,++count);
        lua_pushinteger(L,clients[i]->clientnum);
        lua_settable(L, -3);
    }

    return 1;
}

bool is_player(clientinfo * ci){return ci->state.state != CS_SPECTATOR && ci->state.aitype == AI_NONE;}
bool is_spectator(clientinfo * ci){return ci->state.state == CS_SPECTATOR; /* bots can't be spectators*/}
bool is_bot(clientinfo * ci){return ci->state.aitype != AI_NONE;}
bool is_any(clientinfo *){return true;}

int lua_player_list(lua_State * L){return make_client_list(L, &is_player);}

int lua_spec_list(lua_State * L){return make_client_list(L, &is_spectator);}

int lua_bot_list(lua_State *L){return make_client_list(L, &is_bot);}

int lua_client_list(lua_State * L){return make_client_list(L, &is_any);}

int player_id(lua_State * L)
{
    int cn = luaL_checkint(L, 1);
    clientinfo * ci = get_ci(cn);

    luaL_Buffer buffer;
    luaL_buffinit(L, &buffer);

    if(ci->state.aitype == AI_NONE)
    {
        uint ip_long = getclientip(get_ci(cn)->clientnum);
        luaL_addlstring(&buffer, reinterpret_cast<const char *>(&ip_long), sizeof(ip_long));
        convert2utf8 utf8name(ci->name);
        luaL_addlstring(&buffer, utf8name.str(), utf8name.length());
    }
    else
    {
        luaL_addstring(&buffer, "bot");
        luaL_addlstring(&buffer, reinterpret_cast<const char *>(&ci->sessionid), sizeof(ci->sessionid));
    }

    luaL_pushresult(&buffer);
    return 1;
}

int player_sessionid(int cn)
{
    clientinfo * ci = getinfo(cn);
    return (ci ? ci->sessionid : -1);
}

int player_ownernum(int cn)
{
    return get_ci(cn)->ownernum;
}

const char * player_ip(int cn)
{
    return get_ci(cn)->hostname();
}

std::string player_name(int cn) {
    convert2utf8 utf8name(get_ci(cn)->name);
    return utf8name.stdstr();
}

unsigned long player_iplong(int cn)
{
    return ntohl(getclientip(get_ci(cn)->clientnum));
}

int player_status_code(int cn)
{
    return get_ci(cn)->state.state;
}

int player_ping(int cn)
{
    return get_ci(cn)->ping;
}

int player_ping_update(int cn)
{
    return get_ci(cn)->lastpingupdate;
}

int player_lag(int cn)
{
    return get_ci(cn)->lag;
}

int player_real_lag(int cn)
{
    return totalmillis - get_ci(cn)->lastposupdate;
}

int player_maploaded(int cn)
{
    return get_ci(cn)->maploaded;
}

int player_deathmillis(int cn)
{
    clientinfo * ci = get_ci(cn);
    return ci->state.lastdeath;
}

int player_frags(int cn)
{
    clientinfo * ci = get_ci(cn);
    return ci->state.frags + ci->state.suicides + ci->state.teamkills;
}

int player_score(int cn)
{
    return get_ci(cn)->state.frags;
}

int player_deaths(int cn)
{
    return get_ci(cn)->state.deaths;
}

int player_suicides(int cn)
{
    return get_ci(cn)->state.suicides;
}

int player_teamkills(int cn)
{
    return get_ci(cn)->state.teamkills;
}

int player_damage(int cn)
{
    return get_ci(cn)->state.damage;
}

int player_damagewasted(int cn)
{
    clientinfo * ci = get_ci(cn);
    return ci->state.explosivedamage + ci->state.shotdamage - ci->state.damage;
}

int player_maxhealth(int cn)
{
    return get_ci(cn)->state.maxhealth;
}

int player_health(int cn)
{
    return get_ci(cn)->state.health;
}

int player_gun(int cn)
{
    return get_ci(cn)->state.gunselect;
}

int player_hits(int cn)
{
    return get_ci(cn)->state.hits;
}

int player_misses(int cn)
{
    return get_ci(cn)->state.misses;
}

int player_shots(int cn)
{
    return get_ci(cn)->state.shots;
}

int player_accuracy(int cn)
{
    clientinfo * ci = get_ci(cn);
    int shots = ci->state.shots;
    int hits = shots - ci->state.misses;
    return static_cast<int>(roundf(static_cast<float>(hits)/std::max(shots,1)*100));
}

int player_accuracy2(int cn)
{
    clientinfo * ci = get_ci(cn);
    return static_cast<int>(roundf(static_cast<float>(ci->state.damage*100/max(ci->state.shotdamage,1))));
}

int player_clientmillis(int cn)
{
    clientinfo * ci = get_ci(cn);
    return ci->clientmillis;
}

int player_privilege_code(int cn)
{
    return get_ci(cn)->privilege;
}

const char * player_privilege(int cn)
{
    return privname(get_ci(cn)->privilege);
}

const char * player_status(int cn)
{
    switch(get_ci(cn)->state.state)
    {
        case CS_ALIVE: return "alive";
        case CS_DEAD: return "dead";
        case CS_SPAWNING: return "spawning";
        case CS_LAGGED: return "lagged";
        case CS_SPECTATOR: return "spectator";
        case CS_EDITING: return "editing";
        default: return "unknown";
    }
}

int player_connection_time(int cn)
{
    return (totalmillis - get_ci(cn)->connectmillis)/1000;
}

int player_timeplayed(int cn)
{
    clientinfo * ci = get_ci(cn);
    return (ci->state.timeplayed + (ci->state.state != CS_SPECTATOR ? (lastmillis - ci->state.lasttimeplayed) : 0))/1000;
}

void player_slay(int cn)
{
    clientinfo * ci = get_ci(cn);
    if(ci->state.state != CS_ALIVE) return;
    ci->state.state = CS_DEAD;
    sendf(-1, 1, "ri2", N_FORCEDEATH, cn);
}

bool player_isbot(int cn){return get_ci(cn)->state.aitype != AI_NONE;}

bool player_has_joined_game(int cn)
{
    return get_ci(cn)->connected;
}

void player_join_game(int cn)
{
    clientinfo * ci = get_ci(cn);
    if(!ci->connected)
    {
        connected(ci);
    }
}

void player_reject_join_game(int cn)
{
    clientinfo * ci = get_ci(cn);
    if(!ci->connected)
    {
        disconnect_client(cn, ci->connectauth);
    }
}

void player_msg(int cn, const char * text)
{
    convert2cube textcubeenc(text);
    clientinfo * ci = get_ci(cn);
    if(ci->state.aitype != AI_NONE) return;//TODO assert(false) to catch bugs
    sendf(cn, 1, "ris", N_SERVMSG, textcubeenc.str());
}

void server_msg(const char * text)
{
    convert2cube textcubeenc(text);
    sendservmsg(textcubeenc.str());
}


void player_rename(int cn, const char * newname, bool public_rename)
{
    char safenewname[MAXNAMELEN + 1];
    convert2cube newnamecubeenc(newname);
    filtertext(safenewname, newnamecubeenc.str(), false, MAXNAMELEN);
    if(!safenewname[0]) copystring(safenewname, "unnamed", MAXNAMELEN + 1);

    clientinfo * ci = get_ci(cn);

    if (!ci || cn >= 128) return;

    putuint(ci->messages, N_SWITCHNAME);
    sendstring(safenewname, ci->messages);

    vector<uchar> switchname_message;
    putuint(switchname_message, N_SWITCHNAME);
    sendstring(safenewname, switchname_message);

    packetbuf p(MAXTRANS, ENET_PACKET_FLAG_RELIABLE);
    putuint(p, N_CLIENT);
    putint(p, ci->clientnum);
    putint(p, switchname_message.length());
    p.put(switchname_message.getbuf(), switchname_message.length());
    sendpacket(ci->clientnum, 1, p.finalize(), (public_rename ? -1 : ci->clientnum));

    char oldname[MAXNAMELEN+1];
    copystring(oldname, ci->name, MAXNAMELEN+1);

    copystring(ci->name, safenewname, MAXNAMELEN+1);

    if(public_rename)
    {
        convert2utf8 oldnameutf8(oldname);
        convert2utf8 nameutf8(ci->name);
        event_rename(event_listeners(), std::make_tuple(ci->clientnum, oldnameutf8.str(), nameutf8.str()));
    }
}

} // namespace miaoumod
