#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dict.h"

typedef struct DictNode {
    function func;
    char *key;
    struct DictNode *next;
    struct DictNode *prev;
} DictNode;

typedef struct DictList {
    DictNode *root;
    int numOfFuncs;
} DictList;

static DictNode *getNode(DictList *list, char *key) {
    if (list != NULL) {
        DictNode *toGet = list->root;
        while (toGet != NULL) {
            if (strcmp(toGet->key,key) == 0)
                return toGet;
            toGet = toGet->next;
        }
    }
    return NULL;
}

function getFromDict(DictList *list, char *key) {
    DictNode* toGet = getNode(list,key);
    if(toGet != NULL){
        return toGet->func;
    }
    return NULL;
}

DictList* createDict() {
    DictList *list = (DictList *) malloc(sizeof(struct DictList));
    if (list == NULL)
        return NULL;
    list->numOfFuncs = 0;
    list->root = NULL;
    return list;
}

void insertToDict(DictList *list, function func, char *key) {
    if (list->root == NULL) {
        DictNode *newNode = (DictNode *) malloc(sizeof(struct DictNode));
        if (newNode == NULL)
            return;
        list->root = newNode;
        newNode->func = func;
        newNode->key = key;
        list->root->next = NULL;
        list->root->prev = NULL;
        list->numOfFuncs = 1;
    } else if (getFromDict(list, key) == NULL) {
        DictNode *nextNode = list->root;
        while (nextNode->next != NULL) {
            nextNode = nextNode->next;
        }
        DictNode *newNode = (DictNode *) malloc(sizeof(struct DictNode));
        if (newNode == NULL)
            return;
        nextNode->next = newNode;
        newNode->func = func;
        newNode->key = key;
        newNode->prev = nextNode;
        newNode->next = NULL;
        list->numOfFuncs = list->numOfFuncs + 1;
    }
}

void removeFromDict(DictList* list, char *key) {
    DictNode *toRemove = getNode(list, key);
    if (toRemove != NULL) {
        if (toRemove->prev != NULL && toRemove->next != NULL) {
            toRemove->prev->next = toRemove->next;
            toRemove->next->prev = toRemove->prev;
        } else if (toRemove->prev != NULL && toRemove->next == NULL) {
            toRemove->prev->next = NULL;
        } else if (toRemove->prev == NULL && toRemove->next != NULL) {
            list->root = toRemove->next;
            toRemove->next->prev = NULL;
        } else if (toRemove->prev == NULL && toRemove->next == NULL) {
            list->root = NULL;
        }
        list->numOfFuncs = list->numOfFuncs - 1;
        toRemove->next = NULL;
        toRemove->prev = NULL;
        toRemove->key = NULL;
        toRemove->func = NULL;
        free(toRemove);
    }
}
