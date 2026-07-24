#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasher.h"
#include "table.h"

// Hash table structure
typedef struct HashTable {
    HashTableNode** table;
    int size;
} HashTable;

// Hash table node
typedef struct HashTableNode {
    char* key;
    char* value;
    struct HashTableNode* next;
} HashTableNode;

// Hash table functions
HashTable* createHashTable(int size) {
    // Create a new hash table with the specified size
    HashTable* hashTable = malloc(sizeof(HashTable));
    hashTable->table = malloc(sizeof(HashTableNode*) * size);
    hashTable->size = size;
    for (int i = 0; i < size; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

int hash(char* key, int size) {
    // Simple hash function: sum of ASCII values modulo size
    int hashValue = 0;
    for (int i = 0; i < strlen(key); i++) {
        hashValue += key[i];
    }
    return hashValue % size;
}

void insert(HashTable* hashTable, char* key, char* value) {
    // Insert a new key-value pair into the hash table
    int index = hash(key, hashTable->size);
    HashTableNode* node = hashTable->table[index];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            // Key already exists, update value
            free(node->value);
            node->value = strdup(value);
            return;
        }
        node = node->next;
    }
    // Create a new node and add it to the table
    HashTableNode* newNode = malloc(sizeof(HashTableNode));
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;
}

void delete(HashTable* hashTable, char* key) {
    // Delete a key-value pair from the hash table
    int index = hash(key, hashTable->size);
    HashTableNode* node = hashTable->table[index];
    if (node == NULL) {
        // Key not found, do nothing
        return;
    }
    if (strcmp(node->key, key) == 0) {
        // Key found, remove it
        hashTable->table[index] = node->next;
        free(node->key);
        free(node->value);
        free(node);
        return;
    }
    while (node->next != NULL) {
        if (strcmp(node->next->key, key) == 0) {
            // Key found, remove it
            HashTableNode* nextNode = node->next->next;
            free(node->next->key);
            free(node->next->value);
            free(node->next);
            node->next = nextNode;
            return;
        }
        node = node->next;
    }
}

void printHashTable(HashTable* hashTable) {
    // Print the contents of the hash table
    for (int i = 0; i < hashTable->size; i++) {
        HashTableNode* node = hashTable->table[i];
        while (node != NULL) {
            printf("Key: %s, Value: %s\n", node->key, node->value);
            node = node->next;
        }
    }
}

int main() {
    // Create a new hash table with a size of 10
    HashTable* hashTable = createHashTable(10);

    // Insert some key-value pairs
    insert(hashTable, "key1", "value1");
    insert(hashTable, "key2", "value2");
    insert(hashTable, "key3", "value3");

    // Print the contents of the hash table
    printHashTable(hashTable);

    // Delete a key-value pair
    delete(hashTable, "key2");

    // Print the contents of the hash table again
    printHashTable(hashTable);

    // Free the hash table and its nodes
    for (int i = 0; i < hashTable->size; i++) {
        HashTableNode* node = hashTable->table[i];
        while (node != NULL) {
            HashTableNode* nextNode = node->next;
            free(node->key);
            free(node->value);
            free(node);
            node = nextNode;
        }
    }
    free(hashTable->table);
    free(hashTable);

    return 0;
}