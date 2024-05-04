#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include "filesystem.h"


tree* createBST(){ //creates the file tree and creates the root node, the name of the root is "root", but this name is only used for debugging
    tree*t = (tree*)malloc(sizeof(tree));
    char * name="root";
    node * z=createnode(name, 1);
    t->root=z;
    return t;
}

node* createnode(char * name, int type){ // creates a node and then creates a childlist for the children of the node
    node*n = (node*)malloc(sizeof(node));
    n->type=type;
    n->parent=NULL;
    n->children=createlist();
    n->name=name;
    n->childnode=NULL; //points to the child that connects this node to the parent node
    return n;
}


list* createlist(){
    list*l = (list*)malloc(sizeof(list)); //creates the child list
    l->first==NULL;
    return l;
}

 
node * follow(tree* t, node * cwd, char * path, int remy){ //determines whether the follow should begin at the cwd or root and starts the recursive follow
    node * n;
    node * z;
    
    if(path[0]=='/'){
        path++;
        n=t->root;
        z = recfollow(t, n, path, remy); //follow with root as input for paths beginning with '/'
        if(z==NULL){
            return NULL;
        }
        else{
            return z;
        }
    }
    else{
        n=cwd;
        z = recfollow(t, n, path, remy); //follow with cwd as input for all other paths
        if(z==NULL){
            return NULL;
        }
        else{
            return z;
        }
    }
}

node * recfollow(tree* t, node * n, char * path, int remy){ //recursivly follows the path inputed and returns the end node of the path
   //puts("start recfollow");
    int z=0;
    z++;
    
    if(path[0] == '\0'){
        return n;
    }
    node * newt=NULL;
    node * newnode;
    child * newchild;
    char * token;
    token = (char *) malloc (101);
    child * current;
    int i = 0;
    char * temp;
    
    while(path[i]!='/' && path[i]!='\0'){ //creates token from first segment of path
        
        token[i]=path[i];
        i++;
    }
    token[strlen(token)+1]='\0'; //add endline
    path=path+i+1; //removes token from path
    if(strcmp(".",token)==0){
        newt=n;
    }
    else if(strcmp("..",token)==0){ //recursivly goes up if '..'
        if(n->parent == NULL){
            newt=n;
        }
        else{
        newt=n->parent;
        }
    }
    else if(n->children->first == NULL){
        temp = recPWD(n);
        if(temp[0]=='/' && temp[1]=='\0'){
            printf("Path Error: directory ’/%s’ does not exist.\n", token);
        }
        else{
        printf("Path Error: directory ’%s/%s’ does not exist.\n", temp, token);}
        return NULL;
        
    }
    else{
        current=n->children->first;
        while(current!=NULL){
           if(strcmp(current->e->name,token)==0){
               newt=current->e;
            }
        current=current->next;
        }
    }
    
    if(newt==NULL){
        temp = recPWD(n);
        if(temp[0]=='/' && temp[1]=='\0'){
            printf("Path Error: directory ’/%s’ does not exist.\n", token);
        }
        else{
        printf("Path Error: directory ’%s/%s’ does not exist.\n", temp, token);}
        return NULL;
    }
    if(*path == '\0'){
        if(remy==0){
            if(newt->type==0){
            return NULL;
        }
        else{
            return newt;
        }
        }
        else if(remy==1){
            return newt;
        }
    }
    else{
        if(newt->type==0){
            temp = recPWD(n);
            if(temp[0]=='/' && temp[1]=='\0'){
            printf("Path Error: directory ’/%s’ does not exist.\n", token);
            }
            else{
            printf("Path Error: directory ’%s/%s’ does not exist.\n", temp, token);}
            return NULL;
        }
        else{
            return recfollow(t, newt, path, remy);
        }
    }
}

 


void ls(node * cwd){
    char * path = recPWD(cwd);

    //child * current = cwd->children->first;
    printf("Listing For ’%s’:\n", path);
    //while(current!=NULL){
      //  if(current->e->type == 1)
        //printf("D %s\n", current->e->name);
        //else if(current->e->type == 0)
        //printf("F %s\n", current->e->name);
        //current=current->next;
    //}
}

char * recPWD(node* n){
    char * path = NULL;
    node * current;
    current = n;
    char * slash = "/";
    int len;
    
    while(current->parent != NULL){
        if(path == NULL){
            path=current->name;
        }
        else{
            path = add(slash, path);
            path = add(current->name, path);
        }
        current=current->parent;
    }
    
    if(path == NULL){
            return slash;
        }
    else{
            return add(slash, path);
        }
}

char* add(const char* a, const char* b){
    size_t len = strlen(a) + strlen(b);
    char *newt = (char*)malloc(len * sizeof(char) + 1);
    *newt = '\0';
    return strcat(strcat(newt, a) ,b);
}


