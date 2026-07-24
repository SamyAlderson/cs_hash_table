#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 10
#define HASH_FUNCTION(x) (x % HASH_TABLE_SIZE)

typedef struct HashTable {
    int size;
    int* keys;
    char** values;
} HashTable;

HashTable* hash_table_new() {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->size = HASH_TABLE_SIZE;
    table->keys = (int*) malloc(HASH_TABLE_SIZE * sizeof(int));
    table->values = (char**) malloc(HASH_TABLE_SIZE * sizeof(char*));
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        table->keys[i] = -1;
        table->values[i] = NULL;
    }
    return table;
}

void hash_table_delete(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        if (table->values[i] != NULL) {
            free(table->values[i]);
        }
    }
    free(table->keys);
    free(table->values);
    free(table);
}

int hash_table_hash(int key, HashTable* table) {
    return HASH_FUNCTION(key);
}

int hash_table_insert(int key, char* value, HashTable* table) {
    int index = hash_table_hash(key, table);
    if (table->keys[index] == -1) {
        table->keys[index] = key;
        table->values[index] = strdup(value);
        return 1;
    } else if (table->keys[index] == key) {
        // collision resolution: replace existing value
        free(table->values[index]);
        table->values[index] = strdup(value);
        return 1;
    } else {
        // collision resolution: linear probing
        for (int i = 1; i < HASH_TABLE_SIZE; i++) {
            int probe_index = (index + i) % HASH_TABLE_SIZE;
            if (table->keys[probe_index] == -1) {
                table->keys[probe_index] = key;
                table->values[probe_index] = strdup(value);
                return 1;
            } else if (table->keys[probe_index] == key) {
                // collision resolution: replace existing value
                free(table->values[probe_index]);
                table->values[probe_index] = strdup(value);
                return 1;
            }
        }
        // all slots are occupied, table is full
        fprintf(stderr, "Error: hash table is full\n");
        return 0;
    }
}

int hash_table_delete(int key, HashTable* table) {
    int index = hash_table_hash(key, table);
    if (table->keys[index] == key) {
        free(table->values[index]);
        table->keys[index] = -1;
        table->values[index] = NULL;
        return 1;
    } else {
        // collision resolution: linear probing
        for (int i = 1; i < HASH_TABLE_SIZE; i++) {
            int probe_index = (index + i) % HASH_TABLE_SIZE;
            if (table->keys[probe_index] == key) {
                free(table->values[probe_index]);
                table->keys[probe_index] = -1;
                table->values[probe_index] = NULL;
                return 1;
            }
        }
        // key not found
        fprintf(stderr, "Error: key not found\n");
        return 0;
    }
}

int hash_table_get(int key, HashTable* table) {
    int index = hash_table_hash(key, table);
    if (table->keys[index] == key) {
        return 1;
    } else {
        // collision resolution: linear probing
        for (int i = 1; i < HASH_TABLE_SIZE; i++) {
            int probe_index = (index + i) % HASH_TABLE_SIZE;
            if (table->keys[probe_index] == key) {
                return 1;
            }
        }
        return 0;
    }
}

int main() {
    HashTable* table = hash_table_new();
    hash_table_insert(1, "one", table);
    hash_table_insert(2, "two", table);
    printf("%d\n", hash_table_get(1, table));
    hash_table_delete(2, table);
    return 0;
}