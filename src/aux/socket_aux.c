#include "socket_aux.h"

socket_aux_t* socket_init_and_connect(int port, int* error) {
    socket_aux_t* socket_aux = (socket_aux_t*) malloc(sizeof(socket_aux_t));

    socket_aux->socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_aux->socket_desc < 0){
        *error = ERR_SOCKET_CREATION;
        return NULL;
    }

    socket_aux->socket_addr.sin_family = AF_INET;
    socket_aux->socket_addr.sin_port = htons(port);
    socket_aux->socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(connect(socket_aux->socket_desc, (struct sockaddr*)&socket_aux->socket_addr, sizeof(socket_aux->socket_addr)) < 0){
        *error = ERR_SOCKET_CONNECTION;
        return NULL;
    }

    return socket_aux;
}

socket_aux_t* socket_init_and_listen(int port, int* error) {
    socket_aux_t* socket_aux = (socket_aux_t*) malloc(sizeof(socket_aux_t));

    socket_aux->socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_aux->socket_desc < 0){
        *error = ERR_SOCKET_CREATION;
        return NULL;
    }
    
    // Set port and IP:
    socket_aux->socket_addr.sin_family = AF_INET;
    socket_aux->socket_addr.sin_port = htons(port);
    socket_aux->socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Bind to the set port and IP:
    if(bind(
        socket_aux->socket_desc,
        (struct sockaddr*)&(socket_aux->socket_addr),
        sizeof(socket_aux->socket_addr))<0) {
            *error = ERR_SOCKET_BIND;
            return NULL;
    }
    
    // Listen for clients:
    if(listen(socket_aux->socket_desc, 1) < 0){
        *error = ERR_SOCKET_LISTEN;
        return NULL;
    }
    printf("listening on port %d\n", port);
    return socket_aux;
}



size_t socket_receive(socket_aux_t* socket_aux, char client_msg[MSG_SIZE], int* error) {
    size_t n_bytes;
    n_bytes = recv(socket_aux->socket_desc, client_msg, MSG_SIZE, 0);
    if (n_bytes < 0){
        *error = ERR_SOCKET_RECV;
    }
    return n_bytes;
} 

void socket_send(socket_aux_t* socket_aux, char msg[MSG_SIZE], int* error) {
    if (send(socket_aux->socket_desc, msg, MSG_SIZE, 0) < 0){
        *error = ERR_SOCKET_SEND;
    }
    return;
}

void socket_close(socket_aux_t* socket_aux) {
    close(socket_aux->socket_desc);
}

void socket_accept(socket_aux_t* socket_aux, socket_aux_t* socket_client, int* error) {
    int client_size = sizeof(socket_client->socket_addr);
    socket_client->socket_desc = accept(
        socket_aux->socket_desc, 
        (struct sockaddr*)&(socket_client->socket_addr), 
        &(client_size));
    
    if (socket_client->socket_desc < 0){
        *error = ERR_SOCKET_ACCEPT;
    }
}