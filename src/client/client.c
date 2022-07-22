#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "../aux/socket_aux.h"


char server_msg[MSG_SIZE];
int error;
socket_aux_t* socket_client;
int connection_closed;
sem_t mutex;
int port;
pthread_t print_msg_thread;

void* print_msg(void* arg) {
    while (!connection_closed) {
        socket_receive(socket_client, server_msg, &error);
        if (!connection_closed) printf("Server's response: %s\n", server_msg);
    }

    pthread_exit(NULL);
}

void print_help() {
    printf("commands:\n");
    printf("\n//help\n");
    printf("show this menu\n\n");
    printf("//connect\n");
    printf("connect to server, default port is 2000\n\n");
    printf("//ping\n");
    printf("test connection. receive pong message\n\n");
    printf("//whoami\n");
    printf("get nickname from server\n\n");
}

void connect_to_server() {
    if (!connection_closed) {
        printf("ALREADY CONNECTED!\n");
        return;
    }
    printf("connecting to port %d\n", port);
    socket_client = socket_init_and_connect(port, &error);
    if (!socket_client) {
        printf("Client wasn't able to connect to port %d\n", port);
        return;
    }
    pthread_create(&print_msg_thread, NULL, print_msg, NULL);
    connection_closed = FALSE;
}

int main(int argc, char const *argv[])
{
    port = DEFAULT_PORT;

    if (argc == 2) {
        port = atoi(argv[1]);
    }

    sem_init(&mutex, 0, 1);

    
    

    char client_msg[MSG_SIZE];

    connection_closed = TRUE;
    print_help();
    while (1) {

        // printf("Enter message: ");
        char* read_success = fgets(client_msg, MSG_SIZE , stdin);
        // fscanf(stdin, "%10s", client_msg);
        size_t msg_len = strlen(client_msg);
        if (read_success && client_msg[msg_len - 1] == '\n') client_msg[msg_len - 1] = '\0';
        

        if (!read_success || strcmp(client_msg, "/quit") == 0) {
            if (!connection_closed) socket_send(socket_client, client_msg, &error);
            sem_wait(&mutex);
            connection_closed = TRUE;
            sem_post(&mutex);
            printf("Exiting...\n");
            return 0;
        }
        else if (strcmp(client_msg, "/help") == 0) {
            print_help();
            continue;
        }
        else if (strcmp(client_msg, "/connect") == 0) {
            connect_to_server();
            continue;
        }
    

        if (connection_closed) {
            printf("YOU MUST CONNECT FIRST!!\n");
            continue;
        }
        printf("Trying to send: %s\n", client_msg);
        socket_send(socket_client, client_msg, &error);
        
    }
    
    pthread_join(print_msg_thread, NULL);
    // socket_close(socket_client);
    
    return 0;
}
