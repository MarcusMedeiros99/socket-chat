all: server client

server:
	gcc src/server/server.c src/aux/socket_aux.c -o server

client:
	gcc src/client/client.c src/aux/socket_aux.c -o client
