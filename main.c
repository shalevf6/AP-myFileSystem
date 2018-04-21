#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tree.h"
#include "Dict.h"

typedef struct Tree Tree;
typedef struct DictList DictList;

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';
    str = realloc(str, sizeof(char)*len);
    return str;
}

char* getCommand (char* input){
    char commandSeperator[3] = " /";
    char* afterCommand = strstr(input,commandSeperator);
    if(afterCommand != NULL) {
        char *command = (char *) malloc(sizeof(char) * (strlen(input) - strlen(afterCommand ) + 1));
        strncpy(command, input, strlen(input) - strlen(afterCommand ) + 1);
        command[strlen(input) - strlen(afterCommand)] = '\0';
        return command;
    }
    else {
        if(strcmp(input,"ls") == 0) {
            char *command = (char *) malloc(sizeof(char) * 3);
            strcpy(command,"ls");
            return command;
        }
        else {
            char *afterCommandTemp = strstr(input, " ");
            if(afterCommandTemp != NULL) {
                char *command = (char *) malloc(sizeof(char) * (strlen(input) - strlen(afterCommandTemp) + 1));
                strncpy(command,input,strlen(input) - strlen(afterCommandTemp));
                command[strlen(input) - strlen(afterCommandTemp)] = '\0';
                return command;
            }
            else {
                char *command = (char *) malloc(sizeof(char) * (strlen(input) + 1));
                strcpy(command,input);
                return command;
            }
        }
    }
}

char* getPaths (char* input) {
    char pathSeperator[3] = " /";
    char* path = strstr(input,pathSeperator);
    if(path != NULL) {
        path = path + 1;
        char *afterPath = strstr(path,pathSeperator);
        if (afterPath == NULL) {
            char *pathToSend = (char *) malloc(sizeof(char) * (strlen(path) + 1));
            strcpy(pathToSend, path);
            return pathToSend;
        } else {
            char *pathToSend = (char *) malloc(sizeof(char) * (strlen(path) - strlen(afterPath) + 1));
            strncpy(pathToSend, path, strlen(path) - strlen(afterPath));
            pathToSend[strlen(path) - strlen(afterPath)] = '\0';
            return pathToSend;
        }
    }
    else
        return NULL;
}

char* getLastPath(char* path){
    char *currPath = (char *) malloc(sizeof(char) * (strlen(path) + 1));
    strcpy(currPath,path);
    if(strcmp(currPath,"/") == 0) {
        char ans[2] = "/";
        char *ansp = (char *) malloc(sizeof(char) * 2);
        strcpy(ansp,ans);
        free(currPath);
        return ansp;
    }
    char *temp2;
    temp2 = strrchr(currPath,'/');
    temp2 = temp2 + 1;
    char *ans = (char *) malloc(sizeof(char) * (strlen(temp2) + 1));
    strcpy(ans,temp2);
    free(currPath);
    return ans;
}

int CompareFunction(Element e1, Element e2) {
    char *s1 = (char *) e1;
    char *s2 = (char *) e2;
    int temp = strcmp(s1,s2);
    if(temp > 0)
        return 1;
    if(temp < 0)
        return -1;
    return 0;
}

void FreeFunction(Element e) {
    free(e);
}

void PrintFunction(Element e) {
    char *s = (char *) e;
    char *lastPath = getLastPath(s);
    printf("%s",lastPath);
    free(lastPath);
}

char* LabelFunction(Element e) {
    char *s = (char *) e;
    char sp[strlen(s) + 1];
    strcpy(sp,s);
    char *lastPath = getLastPath(sp);
    return lastPath;
}

void mkdir(void *tree1, char* from, char* to){
    Tree *tree = (Tree *) tree1;
    Element element = (Element) malloc(sizeof(char) * (strlen(from) + 1));
    element = (void*)(strcpy(element,from));
    char* lastPath = getLastPath(from);
    char *fromNew;
    if((strlen(from) - 1) != strlen(lastPath)) {
        fromNew = (char *) malloc(sizeof(char) * (strlen(from) - strlen(lastPath)));
        memcpy(fromNew, from, strlen(from) - strlen(lastPath) - 1);
        fromNew[strlen(from) - strlen(lastPath) - 1] = '\0';
    }
    else {
        fromNew = (char *) malloc(sizeof(char) * (strlen(from) - strlen(lastPath) + 1));
        memcpy(fromNew, from, strlen(from) - strlen(lastPath));
        fromNew[strlen(from) - strlen(lastPath)] = '\0';
    }
    int ans = AddIn(tree,element,fromNew,0);
    if(ans == 0) {
        char *name = FirstBadLocation(tree,from);
        printf("Path %s does not exist\n",name);
        free(name);
    }
    free(lastPath);
}

void touch(void *tree1, char* from, char* to){
    Tree *tree = (Tree *) tree1;
    Element element = (Element) malloc(sizeof(char) * (strlen(from) + 1));
    element = (void*)(strcpy(element,from));
    char* lastPath = getLastPath(from);
    char *fromNew;
    if((strlen(from) - 1) != strlen(lastPath)) {
        fromNew = (char *) malloc(sizeof(char) * (strlen(from) - strlen(lastPath)));
        memcpy(fromNew, from, strlen(from) - strlen(lastPath) - 1);
        fromNew[strlen(from) - strlen(lastPath) - 1] = '\0';
    }
    else {
        fromNew = (char *) malloc(sizeof(char) * (strlen(from) - strlen(lastPath) + 1));
        memcpy(fromNew, from, strlen(from) - strlen(lastPath));
        fromNew[strlen(from) - strlen(lastPath)] = '\0';
    }
    int ans = AddIn(tree,element,fromNew,1);
    if(ans == 0) {
        char *name = FirstBadLocation(tree,from);
        printf("Path %s does not exist\n",name);
        free(name);
    }
    free(lastPath);
}

void ls(void *tree1, char* from, char* to){
    Tree *tree = (Tree *) tree1;
    int ans;
    if(from == NULL)
        ans = Print(tree,"/");
    else
        ans = PrintOnlySons(tree,from);
    if(ans == 0) {
        char *name = FirstBadLocation(tree,from);
        printf("Path %s does not exist\n",name);
        free(name);
    }
}

void lsr(void *tree1, char* from, char* to) {
    Tree *tree = (Tree *) tree1;
    int ans = Print(tree,from);
    if(ans == 0) {
        char *name = FirstBadLocation(tree,from);
        printf("Path %s does not exist\n",name);
        free(name);
    }
}

void cp(void *tree1, char* from, char* to) {
    Tree *tree = (Tree *) tree1;
    char *exists = FirstBadLocation(tree,from);
    if(exists == NULL) {
        char *isFolder = FirstBadLocation(tree,to);
        if(isFolder == NULL){ // path 'to' is up to an existing Directory. Need to add old file's name as son
            char *lastPath = getLastPath(from);
            char l[strlen(lastPath) + 1];
            strcpy(l,lastPath);
            char *newPath = (char *) malloc(sizeof(char) * (strlen(l) + strlen(to) + 2));
            strcpy(newPath,to);
            char slash[2] = "/";
            char* s = slash;
            strcat(newPath,s);
            strcat(newPath,l);
            touch(tree,newPath,NULL);
            free(newPath);
            free(lastPath);
        }
        else { // path 'to' is up to a new file name
            touch(tree,to,NULL);
        }
        free(isFolder);
    }
    else {
        char *name = FirstBadLocation(tree,from);
        printf("Path %s does not exist\n",name);
        free(name);
        free(exists);
    }
}


void rm(void *tree1, char* from, char* to) {
    Tree *tree = (Tree *) tree1;
    if (IsLocationIsLeaf(tree,from) == 0){
        printf("The path %s describes a folder\n",from);
    }
    else {
        int ans = Remove(tree,from);
        if(ans == 0) {
            char *name = FirstBadLocation(tree,from);
            printf("Path %s does not exist\n",name);
            free(name);
        }
    }
}

void rmr(void *tree1, char* from, char* to) {
    Tree *tree = (Tree *) tree1;
    int ans = Remove(tree,from);
    if(ans == 0) {
        char *name = FirstBadLocation(tree,from);
        printf("Path %s does not exist\n",name);
        free(name);
    }
}

void mv(void *tree1, char* from, char* to) {
    Tree *tree = (Tree *) tree1;
    int ans = Remove(tree,from);
    if(ans == 0) {
        char *name = FirstBadLocation(tree,from);
        printf("Path %s does not exist\n",name);
        free(name);
    }
    else {
        char *isFolder = FirstBadLocation(tree,to);
        if(isFolder == NULL){ // path 'to' is up to an existing Directory. Need to add old file's name as son
            char *lastPath = getLastPath(from);
            char l[strlen(lastPath) + 1];
            strcpy(l,lastPath);
            char *newPath = (char *) malloc(sizeof(char) * (strlen(l) + strlen(to) + 2));
            strcpy(newPath,to);
            char slash[2] = "/";
            char* s = slash;
            strcat(newPath,s);
            strcat(newPath,l);
            free(lastPath);
            Element element = (Element) malloc(sizeof(char) * (strlen(newPath) + 1));
            element = (void*)(strcpy(element,newPath));
            char* lastPathTouch = getLastPath(newPath);
            char *fromNew;
            if((strlen(newPath) - 1) != strlen(lastPathTouch)) {
                fromNew = (char *) malloc(sizeof(char) * (strlen(newPath) - strlen(lastPathTouch)));
                memcpy(fromNew, newPath, strlen(newPath) - strlen(lastPathTouch) - 1);
                fromNew[strlen(newPath) - strlen(lastPathTouch) - 1] = '\0';
            }
            else {
                fromNew = (char *) malloc(sizeof(char) * (strlen(newPath) - strlen(lastPathTouch) + 1));
                memcpy(fromNew, newPath, strlen(newPath) - strlen(lastPathTouch));
                fromNew[strlen(newPath) - strlen(lastPathTouch)] = '\0';
            }
            int ansTouch = AddIn(tree,element,fromNew,1);
            if(ansTouch == 0) {
                char *name = FirstBadLocation(tree,to);
                printf("Path %s does not exist\n",name);
                free(name);
                touch(tree,from,NULL);
            }
            free(lastPathTouch);
            free(newPath);
        }
        else { // path 'to' is up to a new file name
            Element element = (Element) malloc(sizeof(char) * (strlen(to) + 1));
            element = (void*)(strcpy(element,to));
            char* lastPathTouch = getLastPath(to);
            char *fromNew;
            if((strlen(to) - 1) != strlen(lastPathTouch)) {
                fromNew = (char *) malloc(sizeof(char) * (strlen(to) - strlen(lastPathTouch)));
                memcpy(fromNew, to, strlen(to) - strlen(lastPathTouch) - 1);
                fromNew[strlen(to) - strlen(lastPathTouch) - 1] = '\0';
            }
            else {
                fromNew = (char *) malloc(sizeof(char) * (strlen(to) - strlen(lastPathTouch) + 1));
                memcpy(fromNew, to, strlen(to) - strlen(lastPathTouch));
                fromNew[strlen(to) - strlen(lastPathTouch)] = '\0';
            }
            int ansTouch = AddIn(tree,element,fromNew,1);
            if(ansTouch == 0) {
                char *name = FirstBadLocation(tree,to);
                printf("Path %s does not exist\n",name);
                free(name);
                touch(tree,from,NULL);
            }
            free(lastPathTouch);
        }
        free(isFolder);
    }
}

void insertAllToDict (DictList* map){
    function a = (void *) (&mkdir);
    insertToDict(map,a,"mkdir");
    function b = (void *) (&touch);
    insertToDict(map,b,"touch");
    function c = (void *) (&ls);
    insertToDict(map,c,"ls");
    function d = (void *) (&lsr);
    insertToDict(map,d,"ls -r");
    function e = (void *) (&cp);
    insertToDict(map,e,"cp");
    function f = (void *) (&rm);
    insertToDict(map,f,"rm");
    function g = (void *) (&rmr);
    insertToDict(map,g,"rm -r");
    function h = (void *) (&mv);
    insertToDict(map,h,"mv");
}
void removeAllFromDict (DictList* map){
    removeFromDict(map,"mkdir");
    removeFromDict(map,"touch");
    removeFromDict(map,"ls");
    removeFromDict(map,"ls -r");
    removeFromDict(map,"cp");
    removeFromDict(map,"rm");
    removeFromDict(map,"rm -r");
    removeFromDict(map,"mv");
}

int checkIfIlegalCommand(char *command,char* fullString){
    if((strcmp(command,"mkdir") != 0) && (strcmp(command,"touch") != 0) && (strcmp(command,"ls") != 0) && (strcmp(command,"ls -r") != 0) &&
       (strcmp(command,"cp") != 0) && (strcmp(command,"rm") != 0) && (strcmp(command,"rm -r") != 0) && (strcmp(command,"mv") != 0)) {
        printf("Unknown command: %s\n", fullString);
        return 1;
    }
    return 0;
}

int checkIfIlegalPath(char *path,char *fullString){
    if(path[0] != '/') {
        printf("Unknown command: %s\n", fullString);
        return 1;
    }
    return 0;
}

void main() {
    printf("Greeting Ladies and Gentlemen!\n");
    char* str;
    char* command = NULL;
    char* from = NULL;
    char* to = NULL;
    printf("@> ");
    str = inputString(stdin, 16);
    Tree* tree = NULL;
    tree = createTree(FreeFunction,CompareFunction,PrintFunction,LabelFunction);
    DictList* map = NULL;
    map = createDict();
    insertAllToDict(map);
    char *strTemp = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(strTemp,str);
    command = getCommand(strTemp);
    from = getPaths(strTemp);
    if(from != NULL) {
        char *toPath = strstr(strTemp,from);
        to = getPaths(toPath);
    }
    else
        to = NULL;
    while(strcmp(command,"exit") != 0) {
        if (from != NULL) {
            if(checkIfIlegalCommand(command,str) != 1 && checkIfIlegalPath(from,str) != 1) {
                if(to != NULL) {
                    if(checkIfIlegalPath(to,str) != 1) {
                        if((strcmp(command,"mv") == 0) || (strcmp(command,"cp") == 0)) {
                            function func = getFromDict(map, command);
                            func(tree, from, to);
                        }
                        else {
                            printf("Unknown command: %s\n", str);
                        }
                    }
                    free(to);
                }
                else {
                    if((strcmp(command,"mv") != 0) && (strcmp(command,"cp") != 0)) {
                        function func = getFromDict(map, command);
                        func(tree, from, NULL);
                    }
                    else {
                        printf("Unknown command: %s\n", str);
                    }
                }
            }
            free(from);
        }
        else {
            if(checkIfIlegalCommand(command,str) != 1) {
                if (strcmp(command, "ls") == 0 && strlen(str) == 2) {
                    function func = getFromDict(map, command);
                    func(tree, from, to);
                } else {
                    printf("Unknown command: %s\n", str);
                }
            }
        }
        if(command != NULL)
            free(command);
        if(str != NULL)
            free(str);
        if(strTemp != NULL)
            free(strTemp);
        printf("@> ");
        str = inputString(stdin, 16);
        strTemp = (char *) malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(strTemp,str);
        command = getCommand(strTemp);
        from = getPaths(strTemp);
        if(from != NULL) {
            char *toPath = strstr(strTemp,from);
            to = getPaths(toPath);
        }
        else
            to = NULL;
    }
    if(command != NULL)
        free(command);
    if(from != NULL)
        free(from);
    if(to != NULL)
        free(to);
    if(str != NULL)
        free(str);
    if(strTemp != NULL)
        free(strTemp);
    Remove(tree,"/");
    if(tree != NULL)
        free(tree);
    removeAllFromDict(map);
    if(map != NULL)
        free(map);


/*
    Tree* tree = createTree(FreeFunction,CompareFunction,PrintFunction,LabelFunction);
    mkdir(tree,"/folder1",NULL);
    mkdir(tree,"/folder2",NULL);
    touch(tree,"/folder1/file11",NULL);
    touch(tree,"/folder1/file12",NULL);
    cp(tree,"/folder1/file11","/folder2/file21");
    ls(tree,"/folder2",NULL);
    ls(tree,"/folder1",NULL);
    Remove(tree,"/");
    if(tree != NULL)
        free(tree);
        */

}
