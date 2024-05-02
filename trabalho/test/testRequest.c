#include "../include/request.h"
#include "../include/parser.h"

int main(int argc,char ** argv){

    const char filename[] = "db";
    /*

    Command comando = "ls -l /etc";

    Request * a = createRequest(1,2,comando);

    printRequest(a);

    writeRequest(filename,a);

    destroyRequest(a);*/

    Request * r = readRequest(filename,-1);

    printRequest(r);   
    
    destroyRequest(r);

    return 0;
}