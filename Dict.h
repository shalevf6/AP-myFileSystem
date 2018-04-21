#ifndef DICT_H
#define DICT_H

#include <stdio.h>
#include <stdlib.h>

typedef void *(*function)(void*, char *, char *);
typedef struct DictList DictList;

DictList* createDict();

void insertToDict(DictList* list, function func, char *key);

void removeFromDict(DictList* list, char *key);

function getFromDict(DictList* list, char *key);

#endif //DICT_H
