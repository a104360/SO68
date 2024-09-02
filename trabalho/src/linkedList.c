#include <stdlib.h>
#include <stdio.h>
#include "linkedList.h"
#include "../include/request.h"
#include <stdlib.h>
#include <stdio.h>
#include "linkedList.h"

struct node {
    void *obj;
    struct node *next;
};

LinkedList *createLinkedList() {
    LinkedList *l = malloc(sizeof(LinkedList));
    if (l == NULL) {
        perror("Failed to create the linked list");
        return NULL;
    }

    l->obj = NULL;
    l->next = NULL;
    return l;
}

void append(LinkedList *list, void *obj, void *(*copy)(void *)) {

    if(list == NULL) list = createLinkedList();

    if (list->obj == NULL) {
        list->obj = copy(obj);
        return;
    }
    LinkedList *stream = list;
    while (stream->next != NULL) stream = stream->next;

    stream->next = createLinkedList();
    stream = stream->next;
    stream->obj = copy(obj);
}

void insert(LinkedList **list, void *obj, void *(*copy)(void *)) {
    if ((*list)->obj == NULL) {
        (*list)->obj = copy(obj);
        return;
    }
    LinkedList *new = createLinkedList();
    new->obj = copy(obj);
    new->next = (*list);
    *list = new;
}

void orderInsert(LinkedList **l, void *obj, int (*cmp)(void *, void *), void *(*copy)(void *)) {
    if (obj == NULL || l == NULL || cmp == NULL) return;
    if ((*l)->obj == NULL) {
        (*l)->obj = copy(obj);
        return;
    }
    if (cmp(obj, (*l)->obj) == -1) {
        LinkedList *newNode = createLinkedList();
        newNode->obj = copy(obj);
        newNode->next = (*l);
        *l = newNode;
        return;
    } 
    LinkedList *prev = (*l);
    LinkedList *curr = (*l)->next;
    while (curr && cmp(obj, curr->obj) == 1) { // Possible optimization here
        prev = curr;
        curr = curr->next;
    }
    LinkedList *new = createLinkedList();
    new->obj = copy(obj);
    new->next = curr;
    prev->next = new;
}

void *pop(LinkedList **l) {
    if (*l == NULL || (*l)->obj == NULL) return NULL; // Check if the list is empty

    if((*l)->next == NULL){
        void * obj = (*l)->obj;
        (*l)->obj = NULL;
        return obj;
    }

    LinkedList *stream = *l;
    LinkedList *prev = NULL;

    while (stream->next != NULL) { // Traverse to the last node
        prev = stream;
        stream = stream->next;
    }

    void *obj = stream->obj; // Get the object to return

    if (prev == NULL) { // If there's only one element in the list
        *l = NULL;
    } else {
        prev->next = NULL;
    }

    free(stream); // Free the last node

    return obj; // Return the object
}


void *popFront(LinkedList **list, void (*destroy)(void *)) {
    if ((*list)->obj == NULL || (*list) == NULL) return NULL;
    void *obj = (*list)->obj;
    LinkedList *aux = (*list);
    (*list) = (*list)->next;
    destroy(aux->obj);
    aux->next = NULL;
    free(aux);
    aux = NULL;
    return obj;
}

void destroyLinkedList(LinkedList *list, void (*destroy)(void *)) {
    while (list) {
        if (list->obj) destroy(list->obj);
        LinkedList *aux = list;
        list = list->next;
        aux->next = NULL;
        free(aux);
        aux = NULL;
    }
}

void printLinkedList(LinkedList *list, void (*print)(void *)) {
    if (!list || !(list->obj)) return; 
    LinkedList *stream = list;
    while (stream) {
        print(stream->obj);
        stream = stream->next;
    }
}
