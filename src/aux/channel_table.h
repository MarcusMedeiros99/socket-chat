#ifndef STR_TABLE_H
#define STR_TABLE_H

#include "channel.h"
#include <malloc.h>
#include <string.h>

#define N_BUCKETS 1000

typedef struct str_table_ {
    char table[N_BUCKETS][CH_NAME_SIZE];
    channel_t* channels[N_BUCKETS];
    unsigned int used_buckets;
} str_table_t;

str_table_t* init_table();

int calc_hash(char* name, unsigned int name_size);

channel_t* insert_name(str_table_t* table, char* name, unsigned int name_size);

int find_name(str_table_t* table, char* name, unsigned int name_size);

#endif