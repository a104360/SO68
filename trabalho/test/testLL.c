#include <stdio.h>
#include <stdlib.h>
#include "../include/request.h"
#include "../include/linkedList.h"

int main(){

    Request * a = createRequest(1,1000,"Eu sou o paulo");
    Request * b = createRequest(2,2000,"E eu sou a ticha");
    Request * c = createRequest(3,3000,"Eu sou o paulo");
    Request * d = createRequest(4,4000,"E eu sou a ticha");

    LinkedList * db = createLinkedList();

    /*insert(&db,a);
    insert(&db,b);
    insert(&db,c);
    insert(&db,d);

    destroyRequest((void *) popFront(&db,destroyRequest));
    destroyRequest((void *) popFront(&db,destroyRequest));*/

    orderInsert(&db,NULL,compareRequest);
    orderInsert(&db,a,NULL);
    orderInsert(NULL,NULL,NULL);

    /*orderInsert(&db,d,compareRequest);
    orderInsert(&db,b,compareRequest);
    orderInsert(&db,a,compareRequest);
    orderInsert(&db,c,compareRequest);*/

    destroyRequest(a);
    destroyRequest(b);
    destroyRequest(c);
    destroyRequest(d);

    printLinkedList(db);

    destroyLinkedList(db,destroyRequest);


    return 0;
}