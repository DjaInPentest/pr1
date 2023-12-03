#include <stdio.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include "stack.h"
#include "queue.h"
#include "hash.h"
#include "set.h"
bool usedSet = false;
bool usedTable = false;
bool usedStack = false;
bool usedQueue = false;
void handleQuery(Set* set, Stack* stack, Queue* queue, HashTable* table, const char* query, const char* ag, const char* hashword) {


    if (strcmp(query, "SPUSH") == 0) { //STACK PUSH
        addStack(stack, ag);
        usedStack = true;
    }
    else if (strcmp(query, "SPOP") == 0) { //STACK POP & READ
        delStack(stack);
        printStack(stack);
        usedStack = true;
    }
    else if (strcmp(query, "SADD") == 0) { //SET ADD
        sadd(set, ag);
        usedSet = true;
    }
    else if (strcmp(query, "SREM") == 0) { //SET REM
        srem(set, ag);
        usedSet = true;
    }
    else if (strcmp(query, "SISMEMBER") == 0) { //SET CHECK FOR ACTUAL
        printf("%d\n", sismember(set, ag));
        usedSet = true;
    }
    else if (strcmp(query, "QPUSH") == 0) { //QUEUE PUSH
        qpush(queue, ag);
        usedQueue = true;
    }
    else if (strcmp(query, "QPOP") == 0) { //QUEUE POP, READ
        qpop(queue);
        printQueue(queue);
        usedQueue = true;
    }
    else if (strcmp(query, "HSET") == 0) { //HASH ADD
        printf("Adding: %s, %s\n", ag, hashword);
        hset(table, ag, hashword);
        usedTable = true;
    }
    else if (strcmp(query, "HDEL") == 0) { //HASH REMOVE
        hdel(table, ag);
        usedTable = true;
    }
    else if (strcmp(query, "HGET") == 0) { //HASH GET
        printf("%s\n", hget(table, ag));
        usedTable = true;
    }
    printf("Query: %s, Key: %s, Value: %s\n", query, ag, hashword); // дебаг
 
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Usage: %s --file file.data --query 'SADD FIRST'\n", argv[0]);
        return 1;
    }

    char* fileName = NULL;
    char* query = NULL;
    char* ag = NULL;
    char* hashword = NULL;
    // Обработка аргументов
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            fileName = argv[i + 1];
        }
        else if (strcmp(argv[i], "--query") == 0 && i + 1 < argc) {
            query = argv[i + 1];
            ag = argv[i + 2];
            hashword = argv[i + 3];
        }
    }

    if (fileName == NULL || query == NULL) {
        printf("Usage: %s --file file.data --query 'SADD FIRST'\n", argv[0]);
        return 1;
    }
    Stack* stack = createStack();
    Queue* queue = createQueue();
    HashTable* table = new_table();    
    Set* set = new_set();


    loadSetFromFile(set, fileName); 
    loadTableFromFile(table, fileName);
    loadStackFromFile(stack, fileName);
    loadQueueFromFile(queue, fileName);
    printf("File: %s\n", fileName);
    printf("Query: %s\n", query);

    handleQuery(set, stack, queue, table, query, ag, hashword);


    if (usedSet) {
        saveSetToFile(set, fileName);
    }

    if (usedTable) {
        saveTableToFile(table, fileName);
    }

    if (usedStack) {
        saveStackToFile(stack, fileName);
    }

    if (usedQueue) {
        saveQueueToFile(queue, fileName);
    }

    return 0;
}
