# Basic File System Shell

## Synopsis

This project is my attempt to create a simple file-system shell in C in order to practice my abilites in creating data structures. My implementation uses a tree and INode like concept where each node has a sibiling pointer and a childlist pointer for managing what other files are in that folder.

## Commands

### The following commands can be used to create and manage files:

* ls - lists out contents of current directory
* ls <path> - lists out contents of specified directory
* pwd - displays path from root to current working directory
* mkdir <path> - creates a directory at the specified path
* touch <path> - creates a generic file of a certain name at the specified path
* cd <path> - changes the current working directory
* rm <path> - removes the specified file or directory
* rm -f <path> -  force removes the specified directory, deleting all sub-directories and their files 
* find <local name> - lists the path to all directories and files with the matching local name.




