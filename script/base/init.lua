package.path = package.path .. ";script/package/?.lua;;script/package/?/init.lua"
package.cpath = package.cpath .. ";lib/?.so;lib/lib?.so"

local _io = io
local _os = os
local _math = math
local _string = string

local luvit = require "luvit" -- Init luvit

io = _io
os = _os
math = _math
string = _string

local event_listener = require("script/base/event")

server = require("script/base/server")

server.event_handler = event_listener.add
server.cancel_handler = event_listener.remove
server.cancel_handlers = event_listener.clear_all
server.create_event_signal = event_listener.create_event
server.cancel_event_signal = event_listener.destroy_event

dofile("script/base/player/object.lua")
dofile("script/base/player/iterators.lua")
dofile("script/base/player/command.lua")
dofile("script/base/player/vars.lua")
dofile("script/base/player/private_vars.lua")

server.mmdb_file ="/usr/share/GeoIP/GeoLite2-City.mmdb"
server.display_country_on_connect = 1


dofile("script/base/mmdb.lua")
dofile("script/base/messages.lua")
dofile("script/base/banner.lua")

dofile("script/base/logevents.lua")
