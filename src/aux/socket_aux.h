#ifndef SOCKET_AUX_H
#define SOCKET_AUX_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>

#define TRUE 1
#define FALSE 0

#define DEFAULT_PORT 2000

#define MSG_SIZE 4096

#define SERVER_SOCKET 1
#define CLIENT_SOCKET 2

#define MAX_CLIENTS 10

#define ERR_SOCKET_CREATION 1
#define ERR_SOCKET_BIND 2
#define ERR_SOCKET_LISTEN 3
#define ERR_SOCKET_RECV 4
#define ERR_SOCKET_SEND 5
#define ERR_SOCKET_ACCEPT 6
#define ERR_SOCKET_CONNECTION 7

typedef struct _socket_aux_t {
    int socket_desc;
    struct sockaddr_in socket_addr;
} socket_aux_t;

socket_aux_t* socket_init_and_connect(int port, int* error);
socket_aux_t* socket_init_and_listen(int port, int* error);
size_t socket_receive(socket_aux_t* socket_aux, char client_msg[MSG_SIZE], int* error);
void socket_send(socket_aux_t* socket_aux, char msg[MSG_SIZE], int* error);
void socket_close(socket_aux_t* socket_aux);
void socket_accept(socket_aux_t* socket_aux, socket_aux_t* socket_client, int* error);

#endif