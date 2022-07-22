#ifndef CHANNEL_H
#define CHANNEL_H

#include <malloc.h>

#define CH_NAME_SIZE 200

typedef struct node_ {
    int thread_id;
    struct node_* next;
} node_t;

typedef struct channel_ {
    node_t* first;
    int admin_id;
    char name[CH_NAME_SIZE];
} channel_t;

channel_t* init_channel(char* name);

node_t* init_node(int thread_id);

void insert_node(channel_t* channel, node_t* node);

node_t* remove_node(channel_t* channel, int thread_id);

#endif