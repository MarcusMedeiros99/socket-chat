#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include "../aux/socket_aux.h"

#define DEFAULT_PORT 2000

#define RECV_PRINT_BEGIN 

sem_t thread_mutex[MAX_CLIENTS];
sem_t n_clients_mutex;
sem_t msg_mutex;
int n_clients;
int is_thread_used[MAX_CLIENTS];
char server_msg[MSG_SIZE];
socket_aux_t* socket_server;
socket_aux_t socket_client[MAX_CLIENTS];

typedef struct thread_arg_t_ {
    int id;
} thread_arg_t;

void* read_socket(void* arg) {
    thread_arg_t thread_arg =  *((thread_arg_t*) arg);
    char client_msg[MSG_SIZE];
    int error;
    char apelido[MSG_SIZE];

    sprintf(apelido,"client %d",thread_arg.id);

    printf("begin thread %d\n", thread_arg.id);

    while (1) {
        sem_wait(thread_mutex + thread_arg.id);
        socket_receive(socket_client + thread_arg.id, client_msg, &error);

        if (strcmp(client_msg, "\\quit") == 0) {
            sem_wait(&n_clients_mutex);
            printf("Exiting...\n");
            is_thread_used[thread_arg.id] = FALSE;
            printf("freeing thread %d\n", thread_arg.id);
            n_clients--;
            socket_close(socket_client + thread_arg.id);
            sem_post(&n_clients_mutex);
            sem_post(thread_mutex + thread_arg.id);
            break;
        }
        printf("RECEIVED (from client %d): %.10s\n", thread_arg.id, client_msg);
        
        sem_wait(&msg_mutex);
        strcpy(server_msg, client_msg);
        if (strcmp(client_msg, "\\ping == 0")) strcpy(server_msg, "pong");


        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (is_thread_used[i]) {
                socket_send(socket_client +i, apelido, &error);
                socket_send(socket_client + i, server_msg, &error);
                printf("sending message to client %d\n", i);
            }
        }
        sem_post(&msg_mutex);
        sem_post(thread_mutex + thread_arg.id);
    }

    free(arg);
    printf("end thread %d\n", thread_arg.id);
    pthread_exit(NULL);
}

// void* write_socket(void* arg) {
//     thread_arg_t* thread_arg = (thread_arg_t*) arg;
//     int error;

//     printf("begin write %d\n", thread_arg->id);

//     while (1) {
//         sem_wait(thread_mutex + thread_arg->id);
//         if (!is_thread_used[thread_arg->id]) {
//             printf("freeing thread %d\n", thread_arg->id);
//             n_clients--;
//             sem_post(&n_clients_mutex);
//             sem_post(thread_mutex + thread_arg->id);
//             break;
//         }

//         printf("sending message to client %d\n", thread_arg->id);
//         socket_send(socket_client + thread_arg->id, server_msg, &error);
        
//         sem_post(thread_mutex + thread_arg->id);
        
//         sleep(1);
//     }
//     free(thread_arg);
//     printf("write_exit\n");
//     pthread_exit(NULL);
// }

void init_mutex() {
    for (int i = 0; i < MAX_CLIENTS; i++) sem_init(thread_mutex + i, 0, 1);
    sem_init(&n_clients_mutex, 0, 1);
    sem_init(&msg_mutex, 0, 1);

}

int get_unused_thread_index() {
    for (int i = 0; i < MAX_CLIENTS; i++) if (!is_thread_used[i]) return i;
    return -1;
}

int main(int argc, char const *argv[])
{
    int port = DEFAULT_PORT;
    int debug_mode = FALSE;
    int has_printed_max_connections = FALSE;


    if (argc >= 2) {
        port = atoi(argv[1]);
    }
    int error = 0;

    init_mutex();

    socket_server = socket_init_and_listen(port, &error);

    if (error) {
        printf("Server wasn't able to start and listen, exiting...");
        return -1;
    }
    
    // char client_msg[MSG_SIZE];
    // char server_msg[MSG_SIZE];


    strcpy(server_msg, "Your message was received");

    
    pthread_t read_thread[MAX_CLIENTS], write_thread[MAX_CLIENTS];
    
    while (1) {
        thread_arg_t* thread_arg = (thread_arg_t*) malloc(sizeof(thread_arg_t));
        sem_wait(&n_clients_mutex);
        thread_arg->id = get_unused_thread_index();
        sem_post(&n_clients_mutex);

        if (thread_arg->id == -1) {
            if (!has_printed_max_connections) {
                printf("Maximum number of connections alreaady used\n");
                has_printed_max_connections = TRUE;
            }
            sleep(5);
            continue;;
        }
        has_printed_max_connections = FALSE;

        socket_accept(socket_server, socket_client + thread_arg->id, &error);

        sem_wait(&n_clients_mutex);
        is_thread_used[thread_arg->id] = TRUE;
        n_clients++;
        sem_post(&n_clients_mutex);
        
        if (error) {
            printf("accept error\n");
            exit(error);
        }
        pthread_create(read_thread + thread_arg->id, NULL, read_socket, thread_arg);
        // pthread_create(write_thread + thread_arg->id, NULL, write_socket, thread_arg);
    }
    
    
    

    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join((pthread_t) (read_thread + i), NULL);
        pthread_join((pthread_t) (write_thread + i), NULL);
    }

    // sem_wait(&thread_mutex);
    // socket_close(socket_server);
    // socket_close(&socket_client);
    // sem_post(&thread_mutex);
    
    // sem_destroy(&thread_mutex);

    return 0;
}
