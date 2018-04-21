#ifndef TREE_H
#define TREE_H
#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0
typedef void* Element;
typedef void (*freeFunction)(Element);
typedef int (*compareFunction)(Element,Element);
typedef void (*printFunction)(Element);
typedef char* (*labelFunction)(Element);

//create Tree
struct Tree* createTree (freeFunction freef,compareFunction compf,printFunction printf,labelFunction labelf);

//adding element of Type to the tree
int Add(struct Tree* tree,Element element,bool isLeaf);

//adding new node of Type to the element
int AddIn (struct Tree* tree,Element element,char* location,bool isLeaf);

//remove the file or folder from the tree
int Remove(struct Tree* tree,char* location);

//put the node in the new place
void Reassign (struct Tree* tree,Element element,char* location);

//print the subtree of the given element
int Print(struct Tree* tree,char *location);

//print only the sons of a given location
int PrintOnlySons(struct Tree* tree,char *location);

//returns a string of the node that doesn't exist first (from the root) from a given location
char* FirstBadLocation(struct Tree* tree,char *location);

//returns 1 if the location is a leaf or 0 if it's not
int IsLocationIsLeaf(struct Tree* tree,char *location);

#endif //TREE_H
