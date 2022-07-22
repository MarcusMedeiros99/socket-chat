all: server client

server:
	gcc src/server/server.c src/aux/socket_aux.c src/aux/channel.c src/aux/channel_table.c -o server -lpthread

client:
	gcc src/client/client.c src/aux/socket_aux.c -o client

clean:
	rm client server
