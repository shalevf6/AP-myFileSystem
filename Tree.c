#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tree.h"

typedef struct Node {
    Element value;
    bool isLeaf;
    char *location;
    int depth;
    int numOfSons;
    struct Node *prevSibling;
    struct Node *nextSibling;
    struct Node *son;
    struct Node *parent;
} Node;

typedef struct Tree {
    Node* root;
    freeFunction freeFunc;
    compareFunction compFunc;
    printFunction printFunc;
    labelFunction labelFunc;
} Tree;

//search for same value of given location recursive
static Node *searchNodeRec(Tree *tree, Node* node, char *location) { // +++++++++++++++++++++++++++++++++++++++++++
    Node *tmp = node;
    Node* found = NULL;
    if (tmp != NULL) {
        if (strcmp(tmp->location, location) == 0)
            return tmp;
        if (tmp->nextSibling != NULL)
            found = searchNodeRec(tree, tmp->nextSibling, location);
        if (found == NULL && tmp->son != NULL)
            found = searchNodeRec(tree, tmp->son, location);
    }
    return found;
}

//search for same value of given location
static Node *searchNode(Tree *tree, char *location) {
    if (strcmp(tree->root->location, location) == 0)
        return tree->root;
    return searchNodeRec(tree, tree->root, location);
}

//create a new node
static Node *createNode() {
    Node *node = (Node *) malloc(sizeof(struct Node));
    if (node == NULL)
        return NULL;
    node->value = NULL;
    node->prevSibling = NULL;
    node->nextSibling = NULL;
    node->son = NULL;
    node->parent = NULL;
    node->isLeaf = 0;
    node->depth = 0;
    node->location = "";
    node->numOfSons = 0;
    return node;
}

//create new tree
Tree *createTree(freeFunction freef,compareFunction compf,printFunction printf,labelFunction labelf) {
    Tree *tree = (Tree *) malloc(sizeof(struct Tree));
    if (tree == NULL)
        return NULL;
    Node *root1 = createNode();
    if (root1 == NULL) {
        free(tree);
        return NULL;
    }
    tree->root = root1;
    char l[2] = "/";
    char *loc = l;
    root1->location = (char *) malloc(sizeof(char) * (strlen(loc) + 1));
    if(root1->location == NULL) {
        free(root1);
        free(tree);
    }
    memcpy(root1->location,loc,strlen(loc));
    root1->location[strlen(loc)] = '\0';
    char e[2] = "/";
    char *elem = e;
    Element element = (Element) malloc(sizeof(char) * (strlen(elem) + 1));
    if(element == NULL) {
        free(root1->location);
        free(root1);
        free(tree);
    }
    element = (void*)(strcpy(element,elem));
    root1->value = element;
    tree->freeFunc = freef;
    tree->compFunc = compf;
    tree->printFunc = printf;
    tree->labelFunc = labelf;
    return tree;
}

static void addInRightPlace(Tree *tree,Node *addThis, Node *asThisSon) { // +++++++++++++++++++++++++++++++++++++++++++
    Node *tmp = asThisSon->son;
    bool finish = false;
    while (!finish && tmp->nextSibling != NULL) {
        if (tree->compFunc(tmp->value, addThis->value) == 1) {   // -1 if tmp<add   1 if tmp>add
            finish = true;
        } else
            tmp = tmp->nextSibling;
    }
    if (tmp->prevSibling == NULL && tmp->nextSibling == NULL){
        if(tree->compFunc(tmp->value, addThis->value) == -1){
            addThis->prevSibling = tmp;
            addThis->nextSibling = NULL;
            addThis->parent = tmp->parent;
            tmp->nextSibling = addThis;
        }
        else{
            addThis->nextSibling = tmp;
            addThis->prevSibling = NULL;
            addThis->parent = tmp->parent;
            addThis->parent->son = addThis;
            tmp->prevSibling = addThis;
        }
    }
    else {
        if (tmp->prevSibling == NULL && tmp->nextSibling != NULL) {  //add before tmp, tmp is first and bigger
            if (tree->compFunc(tmp->value, addThis->value) == -1) {
                addThis->prevSibling = tmp;
                addThis->nextSibling = tmp->nextSibling;
                addThis->parent = tmp->parent;
                tmp->nextSibling = addThis;
            } else {
                addThis->nextSibling = tmp;
                addThis->prevSibling = NULL;
                addThis->parent = tmp->parent;
                addThis->parent->son = addThis;
                tmp->prevSibling = addThis;
            }
        }
        else {
            if (tmp->prevSibling != NULL && tmp->nextSibling == NULL) {  //tmp is the last node
                if (tree->compFunc(tmp->value, addThis->value) == -1) {
                    addThis->prevSibling = tmp;
                    addThis->nextSibling = NULL;
                    addThis->parent = tmp->parent;
                    tmp->nextSibling = addThis;
                }
                else {
                    addThis->nextSibling = tmp;
                    addThis->prevSibling = tmp->prevSibling;
                    addThis->parent = tmp->parent;
                    tmp->prevSibling = addThis;
                }
            } else {
                if (tmp->prevSibling != NULL && tmp->nextSibling != NULL) { //tmp is in now on edge
                    if (tree->compFunc(tmp->value, addThis->value) == -1) {
                        addThis->prevSibling = tmp;
                        addThis->nextSibling = tmp->nextSibling;
                        tmp->nextSibling->prevSibling = addThis;
                        tmp->nextSibling = addThis;
                        addThis->parent = tmp->parent;
                    }
                    else {
                        addThis->nextSibling = tmp;
                        addThis->prevSibling = tmp->prevSibling;
                        tmp->prevSibling->nextSibling = addThis;
                        tmp->prevSibling = addThis;
                        addThis->parent = tmp->parent;
                    }
                }
            }
        }
    }
}

//adding the Node to the right place
int AddIn(Tree *tree, Element element, char *location, bool isLeaf) { // +++++++++++++++++++++++++++++++++++++++++++
    char* temp = tree->labelFunc(element);
    char l[strlen(location) + 1];
    strcpy(l,location);
    Node *isFound = searchNode(tree, location);
    if (isFound == NULL) {
        tree->freeFunc(element);
        free(temp);
        free(location);
        return 0;
    }
    Node *toBeAdded = createNode();
    toBeAdded->isLeaf = isLeaf;
    toBeAdded->value = element;
    toBeAdded->depth = isFound->depth + 1;
    char slash[2] = "/";
    char *s;
    char* newLoc;
    if(strcmp(slash,location) != 0) {
        s = slash;
        newLoc = (char*) malloc(sizeof(char) * (strlen(location) + strlen(temp) + 2));
        strcpy(newLoc,l);
        strcat(newLoc, s);
    }
    else {
        newLoc = (char *) malloc(sizeof(char) * (strlen(location) + strlen(temp) + 1));
        strcpy(newLoc, l);
    }
    char temp1[strlen(temp) + 1];
    strcpy(temp1,temp);
    free(temp);
    strcat(newLoc,temp1);
    toBeAdded->location = newLoc;
    free(location);
    isFound->numOfSons = isFound->numOfSons + 1;
    if (isFound->son == NULL) {
        isFound->son = toBeAdded;
        toBeAdded->parent = isFound;
    } else {
        addInRightPlace(tree, toBeAdded, isFound);
    }
    return 1;
}

//adding the element
int Add(Tree *tree, Element element, bool isLeaf) {
    return AddIn(tree, element, tree->root->location, isLeaf);
}

static void RemoveRec(Tree *tree, Node *node) {
    if (node->son == NULL) {
        if (node->prevSibling != NULL && node->nextSibling != NULL) {
            node->prevSibling->nextSibling = node->nextSibling;
            node->nextSibling->prevSibling = node->prevSibling;
        } else if (node->prevSibling == NULL && node->nextSibling != NULL) {
            node->parent->son = node->nextSibling;
            node->nextSibling->prevSibling = NULL;
        } else if (node->prevSibling != NULL && node->nextSibling == NULL)
            node->prevSibling->nextSibling = NULL;
        else if (node->prevSibling == NULL && node->nextSibling == NULL)
            node->parent->son = NULL;
        tree->freeFunc(node->value);
        node->nextSibling = NULL;
        node->prevSibling = NULL;
        free(node->location);
        free(node);
    } else {
        Node *sons = node->son;
        Node *sonsNext = sons->nextSibling;
        while (sons != NULL) {
            RemoveRec(tree, sons);
            sons = sonsNext;
            if (sonsNext != NULL)
                sonsNext = sons->nextSibling;
        }
        RemoveRec(tree, node);
    }
}

//removing the given element
int Remove(Tree *tree, char *location) {
    Node *node = searchNode(tree, location);
    if (node != NULL) {
        if(node != tree->root)
            if (node->parent != tree->root)
                node->parent->numOfSons = node->parent->numOfSons - 1;
        if (node->son == NULL) {
            if (node != tree->root) {
                if (node->prevSibling != NULL && node->nextSibling != NULL) {
                    node->prevSibling->nextSibling = node->nextSibling;
                    node->nextSibling->prevSibling = node->prevSibling;
                } else if (node->prevSibling == NULL && node->nextSibling != NULL) {
                    node->parent->son = node->nextSibling;
                    node->nextSibling->prevSibling = NULL;
                } else if (node->prevSibling != NULL && node->nextSibling == NULL)
                    node->prevSibling->nextSibling = NULL;
                else if (node->prevSibling == NULL && node->nextSibling == NULL)
                    node->parent->son = NULL;
                tree->freeFunc(node->value);
                node->nextSibling = NULL;
                node->prevSibling = NULL;
                free(node->location);
                free(node);
                return 1;
            } else {
                tree->freeFunc(node->value);
                free(node->location);
                free(node);
            }
        }
        else {
            if(node != tree->root) {
                RemoveRec(tree, node);
                return 1;
            }
            else {
                Node * sons = node->son;
                while(sons != NULL) {
                    Remove(tree,sons->location);
                    sons = node->son;
                }
                tree->freeFunc(node->value);
                free(node->location);
                free(node);
            }

        }
    }
    else
        return 0;
}

//put the node in the new location
void Reassign(Tree *tree, Element element, char *location) {
    Node *whereToChange = searchNode(tree,location);
    int i = Remove(tree,location);
    int tmp = AddIn(tree,element,whereToChange->parent->location,whereToChange->isLeaf);
}

static void printRec(Tree *tree, Node *node) {
    int counter = node->depth;
    while (counter > 1) {
        printf("\t");
        counter = counter - 1;
    }
    tree->printFunc(node->value);
    if (!node->isLeaf) {
        printf(" %d", node->numOfSons);
    }
    printf("\n");
    Node *sons = node->son;
    while (sons != NULL) {
        printRec(tree, sons);
        sons = sons->nextSibling;
    }
}

//printing the tree
int Print(Tree *tree, char *location) {
    Node *node = searchNode(tree, location);
    if (node != NULL) {
        node = node->son;
        while (node != NULL) {
            tree->printFunc(node->value);
            if (!node->isLeaf) {
                printf(" %d", node->numOfSons);
            }
            printf("\n");
            Node *sons = node->son;
            while (sons != NULL) {
                printRec(tree, sons);
                sons = sons->nextSibling;
            }
            node = node->nextSibling;
        }
        return 1;
    }
    else
        return 0;
}

int PrintOnlySons(Tree *tree, char *location) {
    Node *fatherOf = searchNode(tree, location);
    if(fatherOf != NULL) {
        Node *tmp = fatherOf->son;
        while (tmp != NULL) {
            tree->printFunc(tmp->value);
            if (tmp->isLeaf == false)
                printf(" %d\n", tmp->numOfSons);
            else
                printf("\n");
            tmp = tmp->nextSibling;
        }
        return 1;
    }
    else
        return 0;
}

char* FirstBadLocation(Tree *tree,char *location) {
    Node* node = searchNode(tree,location);
    if(node == NULL){
        char* temp1 = (char*)malloc(sizeof(char) * (strlen(location) + 1));
        strcpy(temp1,location);
        char* temp2 = (char*)malloc(sizeof(char) * (strlen(location) + 1));
        strcpy(temp2,location);
        char *temp3 = NULL;
        while(node == NULL) {
            strcpy(temp2,temp1);
            temp3 = strrchr(temp1,'/');
            if(temp3 != NULL)
                *temp3 = '\0';
            else {
                char loc[strlen(location) + 1];
                strcpy(loc,location);
                char *temp4 = strchr(loc,'/');
                temp4 = temp4 + 1;
                char *temp5 = strchr(temp4,'/');
                if(temp5 != NULL)
                    *temp5 = '\0';
                char *ans = (char*)malloc(sizeof(char) * (strlen(temp4) + 1));
                strcpy(ans,temp4);
                free(temp1);
                free(temp2);
                return ans;
            }
            node = searchNode(tree,temp1);
        }
        char *temp4 = strrchr(temp2,'/');
        temp4 = temp4 + 1;
        char *ans = (char*)malloc(sizeof(char) * (strlen(temp4) + 1));
        strcpy(ans,temp4);
        free(temp1);
        free(temp2);
        return ans;
    }
    else
        return NULL;
}

int IsLocationIsLeaf(Tree *tree,char *location){
    Node *temp = searchNode(tree,location);
    if(temp != NULL)
        return temp->isLeaf;
    return 0;
}
