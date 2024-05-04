#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include "filesystem.h"

int main(int argc, char ** argv){
   int k=0;
   int n;
   char command[100];
   tree* t = createBST();
   int bytes_read;
   size_t nbytes = 100;
   char *path;
   char *token;
   const char s[2] = "/";
   path = (char *) malloc (101);
   node * cwd = t->root;
   node * temptwo;
   
   while(k==0){
	 scanf("%s", command);
	        if(strcmp(command, "ls") == 0)
	            {
	                
                    
                       ls(cwd);
             
             
                 
	            }
	        else if(strcmp(command, "pwd") == 0)
                {
                    char * temp;
                    temp = recPWD(cwd);
                    puts(temp);
                 }
                 
            /*else if(strcmp(command, "mkdir") == 0)
                {
                   
                        path++;
                        path[strlen(path)-1]='\0';
                        
                         mkdir(t, cwd, path);
                    
                 }*/
                 /*
            else if(strcmp(command, "touch") == 0)
                {
                   bytes_read = getline (&path, &nbytes, stdin);
                    
                        path++;
                        path[strlen(path)-1]='\0';
                         touch(t, cwd, path);
                   
                 }*/
            else if(strcmp(command, "cd") == 0)
                {
                 // bytes_read = getline (&path, &nbytes, stdin);
                    
                   // if(bytes_read <= 2){
                       //puts("Error: can't change current working directory.");
                   // }
                    //else{
                        path++;
                        path[strlen(path)-1]='\0';
                        temptwo = follow(t, cwd, path, 0);
                        if(temptwo!=NULL){
                            cwd=temptwo;
                        }
                        else if(temptwo==NULL){
                            printf("Change Dir Error: Cannot change working directory.\n");
                        }
                 //}
                 }
            /*else if(strcmp(command, "find") == 0)
                {
                  bytes_read = getline (&path, &nbytes, stdin);
                    
                    if(bytes_read <= 2){
                       //puts("Error: Path Too Short");
                    }
                    else{
                        path++;
                        path[strlen(path)-1]='\0';
                        find(t, cwd, path);
                        
                 }
                 }
            else if(strcmp(command, "rm") == 0)
                {
                   bytes_read = getline (&path, &nbytes, stdin);
                    if(bytes_read <= 2){
                       return;
                    }
                    else{
                        path++;
                        path[strlen(path)-1]='\0';
                        if(path[0]=='-'){
                            if(path[1]=='f'){
                                path=path+3;
                                rmf(t, cwd, path);
                            }
                        }
                        else{
                        rm(t, cwd, path);
                        }
                        
                 }
                 }*/
     }
	return 0;
}