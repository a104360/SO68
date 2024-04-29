#include "../include/request.h"
#include "../include/parser.h"

int main(int argc,char ** argv){

    const char filename[] = "db";

    Command comandos[3] = {"ls","cat","\0"};

    Request * a = createRequest(1,2000,comandos,2);
    Request * b = createRequest(2,4000,comandos,2);
    Request * c = createRequest(3,6000,comandos,2);
    Request * d = createRequest(4,8000,comandos,2);


    writeRequest(filename,a);
    writeRequest(filename,b);
    writeRequest(filename,c);
    writeRequest(filename,d);

    destroyRequest(a);
    destroyRequest(b);
    destroyRequest(c);
    destroyRequest(d);

    Request * r = readRequest(filename,3);
    
    printRequest(r);

    destroyRequest(r);

    return 0;
}