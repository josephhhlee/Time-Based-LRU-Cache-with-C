#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct Node
{
    struct Node *next, *prev;
    int item;
    time_t expiry;
}Node;

typedef struct Hash
{
    int count;
    int size;
    Node *front, *end;
    Node** items;
}Hash;

Hash* createTable(int size, int itemMax) {

    Hash* table = (Hash*)malloc(sizeof(Hash));
    if (table == NULL) {
        exit(0);
    }

    table->size = size;
    table->count = 0;
    table->front = table->end = NULL;

    table->items = (Node**)calloc(itemMax, sizeof(Node*));

    if (table->items == NULL) {
        exit(0);
    }

    for (int i=0; i< itemMax; i++){
        table->items[i] = NULL;
    }
 
    return table;
}

Node* newNode (int item, time_t expiry){

    Node* temp = (Node*)malloc(sizeof(Node));

    if (temp == NULL) {
        exit(0);
    }
    temp->item = item;
    temp->expiry = time(NULL) + expiry;
    temp->next = temp->prev = NULL;

    return temp;
}

int isFull (Hash* hash){
    return hash->size == hash->count;
}

int isEmpty (Hash* hash){
    return hash->end == NULL;
}

int isPresent (Hash* hash, int item){
    return hash->items[item] != NULL;
}

int isFront (Hash* hash, int item){
    return item == hash->front->item;
}

int isEnd (Hash* hash, int item){
    return item == hash->end->item;
}

int isExpiried (Node* node){
    return node->expiry <= time(NULL);
}

void insertNode (Node* node, Hash* hash, int item){

    if (isEmpty(hash)){
        hash->end = hash->front = node;
    } else {
        node->next = hash->front;
        hash->front->prev = node;
        hash->front = node;
    }

    hash->items[item] = node;
    hash->count++;
}

void removePresentNode(Hash* hash, int item){

    Node* temp = NULL;

    if(isFront(hash,item)){
        temp = hash->front;
        hash->front = hash->front->next;
        hash->front->prev = NULL;

    }else if(isEnd(hash,item)){
        temp = hash->end;
        hash->end = hash->end->prev;
        hash->end->next = NULL;

    }else{
        temp = hash->items[item];
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    if(temp != NULL){
        free(temp);
    }

    hash->items[item] = NULL;
    hash->count--;
}

void insertTable (Hash* hash, int item, time_t expiry){

    Node* temp = NULL;

    if(isPresent(hash,item)){
        removePresentNode(hash,item);

    }else if(isFull(hash)){
        temp = hash->end;
        hash->end = hash->end->prev;
        hash->end->next = NULL;
        hash->items[hash->end->item] = NULL;
        hash->count--;
    }

    if(temp != NULL){
        free(temp);
    }

    temp = newNode(item,expiry);
    insertNode(temp,hash,item);
}

Node* getItem(Hash* hash, int item){

    Node* node = hash->items[item];

    if(isExpiried(node)){
        printf("Item [%d] expired\n", item);
        removePresentNode(hash,item);

        return NULL; 
    }

    return node;
}

int main(void){

    Hash* hash = createTable(4, 20); //Declare (Max Number of Items, Max Item Value)

    // insertTable(Hash, Item Value, Expiry in Seconds)
    insertTable(hash, 1, 3); 
    insertTable(hash, 2, 5);
    insertTable(hash, 3, 5);
    insertTable(hash, 1, 1);
    insertTable(hash, 15, 46);
    insertTable(hash, 1, 50);
    insertTable(hash, 1, 12);
    insertTable(hash, 2, 6);
    insertTable(hash, 4, 5);
    insertTable(hash, 1, 8);

    sleep(5);
    getItem(hash, 4);

    
    printf("%d ", hash->end->item);
    printf("%d ", hash->front->item);
    printf("%d ", hash->front->next->item);
    printf("%d ", hash->front->next->next->item);
    // printf("%d ", hash->front->next->next->next->item);

    free(hash->items);
    free(hash);

    return 0;
}
