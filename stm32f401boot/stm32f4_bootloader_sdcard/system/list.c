
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <list.h>

List* createList() {
    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;
    return list;
}

void insert(List* list, char* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = strdup(data);
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->size++;
}

void printList(List* list) {
    Node* current = list->head;
    while (current != NULL) {
        kprintf("%s\n", current->data);
        current = current->next;
    }
}


void freeList(List* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
    free(list);
    //list->size=0;
}

int  getSizeList(List* list){
     return list->size;
}
char* getValueAtIndex(List* list, int index) {
    if (index < 0 || index >= list->size) {
        return NULL;
    }

    Node* current = list->head;
    int currentIndex = 0;
    while (current != NULL) {
        if (currentIndex == index) {
            return current->data;
        }
        current = current->next;
        currentIndex++;
    }

    return NULL;
}
