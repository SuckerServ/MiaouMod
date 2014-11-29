package.path = package.path .. ";script/package/?.lua;;script/package/?/init.lua"
package.cpath = package.cpath .. ";lib/?.so;lib/lib?.so"

local luvit = require "luvit" -- Init luvit

setmetatable(_G, {
  __index = nil,
  __newindex = nil,
})


local event_listener = require("script/base/event")

os = require("os")
math = require("math")
string = require("string")
io = require("io")
table = require("table")

server = require("script/base/server")

server.event_handler = event_listener.add
server.cancel_handler = event_listener.remove
server.cancel_handlers = event_listener.clear_all
server.create_event_signal = event_listener.create_event
server.cancel_event_signal = event_listener.destroy_event

server.log_error = print

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

server.enable_commands({"help", "eval"})
