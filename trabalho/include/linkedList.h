#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct node LinkedList;

/// @brief Aloca memória para a lista ligada sem nenhum elemento
/// @return Pointer para a lista ligada
LinkedList * createLinkedList();

/// @brief Insere o elemento no final da lista
/// @param  lista Pointer para a lista
/// @param  obj Pointer para um objeto de tipo vago
void append(LinkedList *,void *);

/// @brief Insere o elemento na primeira posição da lista
/// @param  listaP Pointer para o pointer para lista
/// @param  obj Pointer para um objeto de tipo vago
void insert(LinkedList **,void *);

/// @brief Insere na lista ordenado segundo uma função de comparação
/// @param  lista Pointer para o pointer para lista
/// @param  obj Pointer para objeto 
/// @param  cmp Pointer para a função de comparação
void orderInsert(LinkedList **,void *,int (*)(void *,void *));


/// @brief Remove e devolve uma cópia do último elemento da cabeça da lista
/// @param  list Pointer para a lista 
/// @param  destroy Pointer para a função de desalocação de memória do objeto
/// @return Pointer para a cópia do objeto
void * pop(LinkedList *,void(*)(void *));


/// @brief Remove e devolve uma cópia do elemento na cabeça da lista
/// @param  list Pointer para o pointer para a lista
/// @param  destroy Pointer para a função de desalocação de memória do objeto
/// @return Pointer para a cópia do objeto
void * popFront(LinkedList **,void(*)(void *));

/// @brief Desaloca os elementos existentes e desaloca a lista ligada
/// @param  list Pointer para a lista
/// @param  destroy Pointer para a função de desalocação de memória do objeto
void destroyLinkedList(LinkedList *,void (*)(void *));

/// @brief Imprime no standard output a lista ligada
/// @param  lista Pointer para a lista
void printLinkedList(LinkedList *);

#endif