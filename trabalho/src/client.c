#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/parser.h"
#include "../include/request.h"
#include <string.h>
#include <stdlib.h>

#define REPLYSIZE 35

static const char fifoComun[] = "../tmp/requestCenter";





/// @brief Retorna o nome do fifo onde vai ser escrito o id do pedido
/// @param pid Id do processo do cliente
/// @return Pointer para o nome do fifo
static char * nameFifo(int pid){
	char * nome = malloc(sizeof(char) * REPLYSIZE);

	snprintf(nome,REPLYSIZE,"../tmp/replyTo_%d",pid);

	return nome;
}







int main(int argc,char ** argv){
	switch (argc)
	{
	case EXECUTE:
		// ../tmp/ReplyTo_<PID>
				
		int id = getpid();
		Request * r = createRequest(id,atoi(argv[2]),argv[4]);

		// Sucesso no envido do request de execução	
		if(writeRequest(fifoComun,r) == -1){
			perror("O request nao foi enviado devidamente");
			return -1;
		}

		// Sucesso
		printf("Request do client %d foi escrito com sucesso",id);

		// Ler IDreply
		//char * cIdFifo = nameFifo(id);

		// Ler output reply

		// Printar output
		break;
	case STATUS:
		// send status to fifo
		return 0;
		break;
	default:
		perror("Formato incorreto, repetir o request");
		return 1;
		break;
	}
	return 0;
}