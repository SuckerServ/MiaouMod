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

const char * player_ip(int cn)
{
    return get_ci(cn)->hostname();
}


void player_msg(int cn, const char * text)
{
    convert2cube textcubeenc(text);
    clientinfo * ci = get_ci(cn);
    if(ci->state.aitype != AI_NONE) return;//TODO assert(false) to catch bugs
    sendf(cn, 1, "ris", N_SERVMSG, text);
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
