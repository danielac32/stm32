

typedef struct Node {
    char* data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    int size;
} List;


List* createList();
void insert(List* list, char* data);
void printList(List* list);
void freeList(List* list);
int  getSizeList(List* list);
char* getValueAtIndex(List* list, int index);
