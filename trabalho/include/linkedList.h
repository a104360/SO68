#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct node LinkedList;

/// @brief Aloca memória para a lista ligada sem nenhum elemento
/// @return Pointer para a lista ligada
LinkedList * createLinkedList();

/// @brief Insere o elemento no final da lista
/// @param  lista Pointer para a lista
/// @param  obj Pointer para um objeto de tipo vago
void append(LinkedList *list, void *obj, void *(*copy)(void *));

/// @brief Insere o elemento na primeira posição da lista
/// @param  listaP Pointer para o pointer para lista
/// @param  obj Pointer para um objeto de tipo vago
void insert(LinkedList **list, void *obj, void *(*copy)(void *));

/// @brief Insere na lista ordenado segundo uma função de comparação
/// @param  lista Pointer para o pointer para lista
/// @param  obj Pointer para objeto 
/// @param  cmp Pointer para a função de comparação
void orderInsert(LinkedList **l, void *obj, int (*cmp)(void *, void *), void *(*copy)(void *));


/// @brief Remove e devolve uma cópia do último elemento da cabeça da lista
/// @param  list Pointer para a lista 
/// @param  destroy Pointer para a função de desalocação de memória do objeto
/// @return Pointer para a cópia do objeto
void *pop(LinkedList **l);


/// @brief Remove e devolve uma cópia do elemento na cabeça da lista
/// @param  list Pointer para o pointer para a lista
/// @param  destroy Pointer para a função de desalocação de memória do objeto
/// @return Pointer para a cópia do objeto
void *popFront(LinkedList **list, void (*destroy)(void *));

/// @brief Desaloca os elementos existentes e desaloca a lista ligada
/// @param  list Pointer para a lista
/// @param  destroy Pointer para a função de desalocação de memória do objeto
void destroyLinkedList(LinkedList *list, void (*destroy)(void *));

/// @brief Imprime no standard output a lista ligada
/// @param  lista Pointer para a lista
void printLinkedList(LinkedList *list, void (*print)(void *));

#endif






