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

server.event_handler("request_auth_challenge", function(cn, name, desc)
	print("[EVENT]: authreq", cn, name, desc)
end)

server.event_handler("auth_challenge_response", function(cn, name, desc)
        print("[EVENT]: authrep", cn, name, desc)
end)

