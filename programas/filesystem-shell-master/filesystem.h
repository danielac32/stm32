#ifndef ____Map_H____
#define ____Map_H____

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include "filesystem.h"

//============ File Tree Structures ============

typedef int element;


struct _node{
  struct _node * parent;
  struct _childlist * children;
  struct _childnode* childnode;
  int type; //0 == File 1 == Directory 
  char * name;
};

typedef struct _node node;

struct _childnode{
  node * e;
  struct _childnode* next;
  struct _childnode* prev;
};

typedef struct _childnode child;

struct _childlist{
  child * first;
};

typedef struct _childlist list;

struct _tree{
  node * root;
};

typedef struct _tree tree;



/*
tree* createBST();
node* createnode(char * name, int type);
list* createlist();
node * follow(tree* t, node * cwd, char * path, int remy);
node * recfollow(tree* t, node * n, char * path, int remy);
char * recPWD(node* n);
char* add(const char* a, const char* b);



*/


tree* createBST();
void insertnode(tree * t, node * n);
node* recinsert(node* root, node *n);
void removenode(tree * t, element e);
node* recremove(node* root, element e);
node * minVal(node* node);
node* createnode(char * name, int type);
void deletenode(node * n);
void postorder(tree *t);
void recpostorder(node* n);
void preorder(tree *t);
void recpreorder(node* n);
void inorder(tree *t);
void recinorder(node* n);
int calculate(tree *t);
int reccalculate(node* n);
void clear(tree *t);
void recclear(node *n);
void mkdir(tree* t, node* cwd, char * path);
node * follow(tree* t, node * cwd, char * path, int remy);
node * recfollow(tree* t, node * n, char * path, int remy);
list* createlist();
child* createchild(node * newnode);
node * create(tree* t, node * cwd, char * path, int type);
node * reccreate(tree* t, node * n, char * path, int type);
int compare(char * L, char * R);
int order(char c);
void ls(node * cwd);
char * recPWD(node* n);
char* add(const char* a, const char* b);
void touch(tree* t, node* cwd, char * path);
void rm(tree* t, node* cwd, char * path);
void find(tree* t, node* cwd, char * token);
void recfind(tree* t, node* n, char * token);


#endif