#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../aux/socket_aux.h"

int main(int argc, char const *argv[])
{
    int port = DEFAULT_PORT;

    if (argc == 2) {
        port = atoi(argv[1]);
    }

    int error;
    socket_aux_t* socket_client = socket_init_and_connect(port, &error);

    if (error) {
        printf("Client wasn't able to start , exiting...");
    }

    char client_msg[MSG_SIZE];
    char server_msg[MSG_SIZE];
    while (1) {

        printf("Enter message: ");
        gets(client_msg);
        
        if (strcmp(client_msg, "\\quit") == 0) {
            printf("Exiting...");
            break;
        }

        socket_send(socket_client, client_msg, &error);
        socket_receive(socket_client, server_msg, &error);
        
        printf("Server's response: %s\n", server_msg);
    }
    
    socket_close(socket_client);
    
    return 0;
}
