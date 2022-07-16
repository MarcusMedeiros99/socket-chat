#include <stdio.h>
#include <stdlib.h>
#include "../aux/socket_aux.h"

#define DEFAULT_PORT 2000

int main(int argc, char const *argv[])
{
    int port = DEFAULT_PORT;

    if (argc == 2) {
        port = atoi(argv[1]);
    } 
    int error = 0;
    socket_aux_t* socket_server = socket_init_and_listen(port, &error);

    if (error) {
        printf("Server wasn't able to start and listen, exiting...");
        return -1;
    }
    
    
    socket_aux_t socket_client;
    socket_accept(socket_server, &socket_client, &error);
    
    char client_msg[MSG_SIZE];
    char server_msg[MSG_SIZE] = "Your message was received";

    while(1) {
        socket_receive(&socket_client, client_msg, &error);
        printf("Received: %s\n", client_msg);
        socket_send(&socket_client, server_msg, &error);
    }
    socket_close(socket_server);
    socket_close(&socket_client);
    
    return 0;
}
