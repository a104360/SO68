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
    Request * b = createRequest(2,10,"antonio");
    Request * c = createRequest(3,13,"antonio");
    Request * d = createRequest(4,1,"antonio");

    LinkedList * l = createLinkedList();

    orderInsert(&l,(void *) a,compareRequest);
    orderInsert(&l,(void *) b,compareRequest);
    orderInsert(&l,(void *) c,compareRequest);
    orderInsert(&l,(void *) d,compareRequest);


    destroyRequest(a);
    destroyRequest(b);
    destroyRequest(c);
    destroyRequest(d);

    //int fd = open
    listToSchedule(&l);

    destroyLinkedList(l,destroyRequest);

    l = createLinkedList();

    updateScheduleFile(l);

    printLinkedList(l);

    destroyLinkedList(l,destroyRequest);

    return 0;
}