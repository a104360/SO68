#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../include/linkedList.h"
#include "../include/request.h"

int main(){

    Request * a = createRequest(1,20,"antonio");
    //Request * b = createRequest(2,10,"antonio");
    //Request * c = createRequest(3,13,"antonio");
    //Request * d = createRequest(4,1,"antonio");

    LinkedList * l = createLinkedList();

    append(l,(void *) a,copyRequest);
    Request * aux = pop(&l);
    //append(l,b);
    //append(l,c);
    //append(l,d);

    destroyRequest(a);
    //destroyRequest(b);
    //destroyRequest(c);
    //destroyRequest(d);

    //int fd = open

    printLinkedList(l,printRequest);

    printRequest(aux);

    destroyRequest(aux);

    destroyLinkedList(l,destroyRequest);
    /*destroyLinkedList(l,destroyRequest);

    l = createLinkedList();

    printLinkedList(l);

    destroyLinkedList(l,destroyRequest);*/

    return 0;
}
