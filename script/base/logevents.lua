server.event_handler("init", function()
	print("[EVENT]: init")
end)

server.event_handler("text", function(cn, text)
	print("[EVENT]: text", cn, text)
end)

server.event_handler("connecting", function(cn, hostname, name, pwd)
        print("[EVENT]: connecting", cn, hostname, name, pwd)
end)

server.event_handler("failedconnect", function(hostname)
        print("[EVENT]: failedconnect", hostname)
end)

server.event_handler("disconnect", function(hostname)
        print("[EVENT]: disconnect", hostname)
end)

server.event_handler("connect", function(hostname)
        print("[EVENT]: connect", hostname)
end)

server.event_handler("editpacket", function(cn)
	print("[EVENT]: editpacket", cn)
end)

server.event_handler("maploaded", function(cn)
        print("[EVENT]: maploaded", cn)
end)

server.event_handler("shutdown", function(id)
	print("[EVENT]: shutdown", id)
end)
