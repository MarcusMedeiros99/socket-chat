#include "channel_table.h"

str_table_t* init_table() {
    str_table_t* table = (str_table_t*) malloc(sizeof(str_table_t));
    memset(table, (int) '\0', N_BUCKETS*CH_NAME_SIZE);
    for (int i = 0; i < N_BUCKETS; i++) table->channels[i] = NULL;

    table->used_buckets = 0;
    
    return table;
}

int calc_hash(char* name, unsigned int name_size) {//bad hash function
    int answer = 0;
    for (int i = 0; i < name_size && CH_NAME_SIZE; i++) {
        answer += (int) name[i];
    }
    return answer;
}

channel_t* insert_name(str_table_t* table, char* name, unsigned int name_size) {
    int find = find_name(table, name, name_size);
    if (
        name_size > CH_NAME_SIZE || 
        table->used_buckets == N_BUCKETS
    ) return NULL;
    
    int hash = calc_hash(name, name_size);

    if (find == -1) {
        int i;
        for (i = 0; i < N_BUCKETS; i++) {
            if (table->table[(hash+i)%N_BUCKETS][0] == '\0') break;
        }
        strncpy(table->table[(hash+i)%N_BUCKETS], name, name_size);
        if (name_size != CH_NAME_SIZE) table->table[(hash+i)%N_BUCKETS][name_size] = '\0';

        channel_t* channel = init_channel(name);
        table->channels[(hash+i)%N_BUCKETS] = channel;

        table->used_buckets++;
        return channel;
    }
    
    if (find != -1) return table->channels[find];
}

int find_name(str_table_t* table, char* name, unsigned int name_size) {
    unsigned int hash = calc_hash(name, name_size);
    int index = -1;

    for (int i = 0; i < N_BUCKETS; i++) {
        if (strncmp(table->table[(hash + i)%N_BUCKETS], name, name_size) == 0) return hash+i;
        if (table->table[(hash+i)%N_BUCKETS][0] == '\0') return -1;
    }
    return -1;
}
