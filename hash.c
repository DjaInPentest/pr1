#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 1000

typedef struct HashEntry {
    char* key;
    char* value;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    HashEntry** entries;
} HashTable;

unsigned int hash_function(const char* key) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash = hash * 31 + key[i];
    }
    return hash % TABLE_SIZE;
}

HashEntry* new_entry(const char* key, const char* value) {
    HashEntry* entry = malloc(sizeof(HashEntry));
    if (entry == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    entry->key = strdup(key);
    if (entry->key == NULL) {
        printf("Memory allocation failed for key\n");
        free(entry);
        return NULL;
    }

    entry->value = strdup(value);
    if (entry->value == NULL) {
        printf("Memory allocation failed for value\n");
        free(entry->key);
        free(entry);
        return NULL;
    }

    entry->next = NULL;
    return entry;
}


HashTable* new_table() {
    HashTable* table = malloc(sizeof(HashTable));
    table->entries = calloc(TABLE_SIZE, sizeof(HashEntry*));
    return table;
}

void hset(HashTable* table, const char* key, const char* value) {
    unsigned int index = hash_function(key);
    printf("Index: %d\n", index); // добавим вывод

    if (table->entries[index] == NULL) {
        printf("Creating new entry\n"); // добавим вывод
        table->entries[index] = new_entry(key, value);
    }
    else {
        printf("Collision detected\n"); // добавим вывод
        HashEntry* entry = table->entries[index];
        while (entry != NULL) {
            if (strcmp(entry->key, key) == 0) {
                free(entry->value);
                entry->value = strdup(value);
                return;
            }
            if (entry->next == NULL) {
                break;
            }
            entry = entry->next;
        }
        entry->next = new_entry(key, value);
    }
}


void hdel(HashTable* table, const char* key) {
    unsigned int index = hash_function(key);
    HashEntry* entry = table->entries[index];
    HashEntry* prev = NULL;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (prev == NULL) {
                table->entries[index] = entry->next;
            }
            else {
                prev->next = entry->next;
            }
            free(entry->key);
            free(entry->value);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

char* hget(HashTable* table, const char* key) {
    unsigned int index = hash_function(key);
    HashEntry* entry = table->entries[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void free_table(HashTable* table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashEntry* entry = table->entries[i];
        while (entry != NULL) {
            HashEntry* prev = entry;
            entry = entry->next;
            free(prev->key);
            free(prev->value);
            free(prev);
        }
    }
    free(table->entries);
    free(table);
}

void saveTableToFile(HashTable* table, const char* filename) {
    printf("Inside saveTableToFile\n");
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file for writing\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashEntry* entry = table->entries[i];
        while (entry != NULL) {
            printf("Writing to file: %s,%s\n", entry->key, entry->value);
            fprintf(file, "%s,%s\n", entry->key, entry->value);
            entry = entry->next;
        }
    }

    fclose(file);
    printf("File writing finished\n");
}

void loadTableFromFile(HashTable* table, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File does not exist. Creating a new file...\n");
        saveTableToFile(table, filename);  // создание нового файла при отсутствии
        return;
    }

    char buffer[255];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Убираем символ новой строки

        char* key = strtok(buffer, ",");
        char* value = strtok(NULL, ",");

        hset(table, key, value);
    }

    fclose(file);
}

/*
int main() {
    HashTable* table = new_table();

    // Загрузка хеш-таблицы из файла при запуске
    loadTableFromFile(table, "hash_table_data.txt");

    hset(table, "key1", "value1");
    hset(table, "key2", "value2");
    hset(table, "k3", "value3");
    printf("%s\n", hget(table, "key1"));
    printf("%s\n", hget(table, "key2"));
    printf("%s\n", hget(table, "k3"));
    hdel(table, "key2");
    printf("%s\n", hget(table, "key1"));
    printf("%s\n", hget(table, "key2"));
    printf("%s\n", hget(table, "key2"));
    printf("%s\n", hget(table, "k"));
    printf("%s\n", hget(table, "y2"));
    printf("%s\n", hget(table, "laba"));

    // Сохранение хеш-таблицы в файл перед завершением программы
    saveTableToFile(table, "hash_table_data.txt");

    free_table(table);
    return 0;
}
*/