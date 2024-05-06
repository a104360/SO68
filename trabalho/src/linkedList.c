#include <stdlib.h>
#include <stdio.h>
#include "linkedList.h"
#include "../include/request.h"

struct node{
    void * obj;
    struct node * next;
};

LinkedList * createLinkedList(){
    LinkedList * l = malloc(sizeof(struct node));
    if(l == NULL){
        perror("Não foi possivel criar a lista ligada");
        return NULL;
    }

    l->obj = NULL;
    l->next = NULL;
    return l;
}

void append(LinkedList * list,void * obj){
    if(list->obj == NULL){
        list->obj = copyRequest((Request *) obj); // TROCAR ESTA FUNÇÃO POR UMA GENERICA
        return;
    }
    LinkedList * stream = list;
    while(stream->next != NULL) stream = stream->next;

    stream->next = createLinkedList();

    stream = stream->next;
    stream->obj = copyRequest((Request *) obj); // TROCAR ESTA FUNÇÃO POR UMA GENERICA
}

void insert(LinkedList ** list,void * obj){
    if((*list)->obj == NULL){
        (*list)->obj = copyRequest((Request *) obj); // TROCAR ESTA FUNÇÃO POR UMA GENERICA
        return;
    }
    LinkedList * new = createLinkedList();

    new->obj = copyRequest((Request *) obj); // TROCAR ESTA FUNÇÃO POR UMA GENERICA
    new->next = (*list);
    *list = new;
}

void orderInsert(LinkedList **l,void * obj,int (*cmp)(void *,void *)){
    if(obj == NULL || l == NULL || cmp == NULL) return;
    if((*l)->obj == NULL){
        (*l)->obj = copyRequest((Request *) obj); // TROCAR ESTA FUNÇÃO POR UMA GENERICA
        return;
    }
    if(cmp(obj,(*l)->obj) == -1){
        LinkedList * newNode = createLinkedList();
        newNode->obj = copyRequest((Request *)obj);
        newNode->next = (*l);
        *l = newNode;
        return;
    } 
    LinkedList * prev = (*l);
    LinkedList * curr = (*l)->next;
    while(curr && cmp(obj,curr->obj) == 1){ // Possivel otimização aqui
        prev = curr;
        curr = curr->next;
    }
    LinkedList * new = createLinkedList();
    new->obj = copyRequest((Request *) obj); // TROCAR ESTA FUNÇÃO POR UMA GENERICA
    new->next = curr;
    prev->next = new;
}

void * pop(LinkedList *l,void (*destroy)(void *)){
    if(l->obj == NULL || l == NULL) return NULL;
    LinkedList * stream = l;
    while(stream->next->next != NULL){
        stream = stream->next;
    }
    LinkedList * aux = stream;
    stream = stream->next;
    void * obj = copyRequest((Request *) stream->obj); // TROCAR ESTA FUNÇÃO POR UMA GENERICA
    if(stream->obj){
        destroy(stream->obj);
        stream->obj = NULL;
    }
    free(stream);
    stream = NULL;
    aux->next = NULL;
    return obj;
}

void * popFront(LinkedList ** list,void(*destroy)(void*)){
    if((*list)->obj == NULL || (*list) == NULL) return NULL;
    void * obj = copyRequest((Request *) (*list)->obj); // TROCAR ESTA FUNÇÃO POR UMA GENERICA
    LinkedList * aux = (*list);
    (*list) = (*list)->next;
    destroy(aux->obj);
    aux->next = NULL;
    free(aux);
    aux = NULL;
    return obj;
}

void destroyLinkedList(LinkedList * list,void (*destroy)(void *)){
    while(list){
        if(list->obj) destroy(list->obj);
        LinkedList * aux = list;
        list = list->next;
        aux->next = NULL;
        free(aux);
        aux = NULL;
    }
}

void printLinkedList(LinkedList * list){
    if(!list || !(list->obj)) return; 
    LinkedList * stream = list;
    while(stream){
        printRequest((Request *) stream->obj);
        stream = stream->next;
    }
}