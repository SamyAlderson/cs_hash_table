#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasher.h"

#define INITIAL_SIZE 16
#define LOAD_FACTOR 0.75

typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

typedef struct Table {
    Node **buckets;
    int size;
} Table;

Table *table_new(void) {
    Table *table = malloc(sizeof(Table));
    if (table == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    table->buckets = calloc(INITIAL_SIZE, sizeof(Node *));
    if (table->buckets == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    table->size = INITIAL_SIZE;
    return table;
}

void table_resize(Table *table) {
    int new_size = table->size * 2;
    Node **new_buckets = calloc(new_size, sizeof(Node *));
    if (new_buckets == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < new_size; i++) {
        new_buckets[i] = NULL;
    }
    for (int i = 0; i < table->size; i++) {
        Node *node = table->buckets[i];
        while (node != NULL) {
            Node *next = node->next;
            int index = hasher(node->key) % new_size;
            node->next = new_buckets[index];
            new_buckets[index] = node;
            node = next;
        }
    }
    free(table->buckets);
    table->buckets = new_buckets;
    table->size = new_size;
}

int table_hash(const char *key) {
    int sum = 0;
    while (*key != '\0') {
        sum += *key++;
    }
    return sum % INITIAL_SIZE;
}

void table_insert(Table *table, const char *key, const char *value) {
    if (table->buckets[hasher(key) % table->size] != NULL) {
        for (Node *node = table->buckets[hasher(key) % table->size]; node != NULL; node = node->next) {
            if (strcmp(node->key, key) == 0) {
                node->value = strdup(value);
                if (node->value == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                return;
            }
        }
    }
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->key = strdup(key);
    if (node->key == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->value = strdup(value);
    if (node->value == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->next = table->buckets[hasher(key) % table->size];
    table->buckets[hasher(key) % table->size] = node;
    double load_factor = (double) table->size / table->size * LOAD_FACTOR;
    if (load_factor > 1) {
        table_resize(table);
    }
}

void table_remove(Table *table, const char *key) {
    for (int i = 0; i < table->size; i++) {
        Node *node = table->buckets[i];
        while (node != NULL) {
            if (strcmp(node->key, key) == 0) {
                Node *next = node->next;
                if (node->value != NULL) {
                    free(node->value);
                }
                if (node->key != NULL) {
                    free(node->key);
                }
                free(node);
                table->buckets[i] = next;
                return;
            }
            node = node->next;
        }
    }
}

char *table_get(Table *table, const char *key) {
    int index = hasher(key) % table->size;
    for (Node *node = table->buckets[index]; node != NULL; node = node->next) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
    }
    return NULL;
}
```

```c
#include <stdio.h>
#include "table.h"

int main(void) {
    Table *table = table_new();
    table_insert(table, "key1", "value1");
    table_insert(table, "key2", "value2");
    printf("%s\n", table_get(table, "key1"));
    table_remove(table, "key1");
    printf("%s\n", table_get(table, "key1"));
    return 0;
}