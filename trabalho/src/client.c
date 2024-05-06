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


static const char fifoComun[] = "../tmp/requestCenter";




int main(int argc,char ** argv){
	switch (argc)
	{
	case 1:
		Request * r = createRequest(-404,0,NULL);
		if(writeRequest(fifoComun,r) == -1){
			perror("O envio do request falhou");
			return -1;
		}
		break;
	case EXECUTE:
		// ../tmp/ReplyTo_<PID>
				
		int id = getpid();
		r = createRequest(id,atoi(argv[2]),argv[4]);

		// Sucesso no envido do request de execução	
		if(writeRequest(fifoComun,r) == -1){
			perror("O request nao foi enviado devidamente");
			return -1;
		}

		char * reply = nameFifo(id);
		int fdReply = open(reply,O_RDONLY);
		char * buffer = malloc(sizeof(char) * 25);
		read(fdReply,buffer,25);
		close(fdReply);
		write(STDOUT_FILENO,buffer,25);
		free(buffer);
		free(reply);
		// Sucesso
		printf("Request do client %d foi escrito com sucesso\n",id);

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