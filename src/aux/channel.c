#include "channel.h"

#include <string.h>

channel_t* init_channel(char* name) {
    channel_t* channel = (channel_t*) malloc(sizeof(channel_t));
    channel->first = NULL;
    strncpy(channel->name, name, CH_NAME_SIZE);
    channel->admin_id = -1;

    return channel;
}

node_t* init_node(int thread_id) {
    node_t* node = (node_t*) malloc(sizeof(node_t));
    node->thread_id = thread_id;
    node->next = NULL;

    return node;
}

void insert_node(channel_t* channel, node_t* node) {
    if (channel->first == NULL) {
        channel->first = node;
        node->next = NULL;
    }
    else {
        node_t* aux = channel->first;
        channel->first = node;
        node->next = aux;
    }
}

node_t* remove_node(channel_t* channel, int thread_id) {
    node_t* first = channel->first;

    if(first) {
        node_t* previous = NULL;
        node_t* current = first;
        while (current != NULL && current->thread_id != thread_id) {
            previous = current;
            current = current->next;
        }

        if (current->thread_id == thread_id) {
            if(current == first) {
                channel->first = first->next;
            }
            else {
                previous->next = current->next;
            }
            current->next = NULL;
        }
        return current;
    }
    return NULL;
}