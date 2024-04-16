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

const char * clientIDFile = "../tmp/clientID.txt";

/// @brief Ler o id a ser atribuido ao cliente atual, guardado em ficheiro
/// @return id a ser atribuido
static int getClientId(){
	int fd = open(clientIDFile,O_CREAT | O_RDONLY); 		// Abrir o ficheiro

	int id = 1;
	lseek(fd,0,SEEK_SET); 									// ir para o inicio do ficheiro
	if(read(fd,&id,sizeof(int)) == 0) return 0;				// ler um inteiro do ficheiro

	close(fd); 												// Fechar o ficheiro 
	return id;
}


/// @brief Atualiza o id guardado em ficheiro para o valor uma unidade acima (id+1)
/// @param clientID /ID do cliente atual
static void updateClientID(int clientID){
	int fd = open(clientIDFile,O_CREAT | O_WRONLY); 	// Abrir o ficheiro para escrita

	lseek(fd,0,SEEK_SET);								// Ir para inicio do ficheiro
	clientID += 1; 										// Incrementar o clientID
	write(fd,&clientID,sizeof(int));					// Escrever no ficheiro o ID atualizado

	close(fd); 											// Fechar o file descriptor 
}

/// @brief Criar um nome para o fifo com o id
/// @param id Id do client que ira enviar o request
/// @param line Pointer para a variável onde o nome será guardado
static void nomearFifo(int id,char * line){
	snprintf(line,"../tmp/%d",id);
}

static void nomeReplyFifo(int id,char * line){
	sprintf(line,"../tmp/replyTo_%d",id);
}

/// @brief Either certifies that the fifo is opened or returns an error (-1)
/// @param nome Pointer para a variável que contém o nome/path para o fifo
/// @return File descriptor do fifo | Em caso de erro, -1
static int openFifo(char * nome){
	int requestFifo = open(nome,O_WRONLY);
		
		if(requestFifo == -1){
			mkfifo(nome,0666);
			requestFifo = open(nome,O_WRONLY);
		}
	return requestFifo;
}

/// @brief Lê chunks de 128 characteres do fifo de resposta ao seu request
/// @param clientID Id do client que efetuou o request
/// @return Pointer para o nome lido do fifo de resposta
static char * readReply(int clientID){
	char * reply;

}

static void readReply(int clientID){
	char * nomeReply;
	nomeReplyFifo(clientID,nomeReply);

	int replyfd = open(nomeReply,O_RDONLY);

	while	
}


static int sendExecuteRequest(int clientID,int argc,char ** argv){
	Request * request = NULL;

	char * nomeFifo;

	if(strncasecmp(argv[3],"-u",sizeof("-u")) == 0){
		Command command = parseUniqueCommand(argv[4]);
		request = createRequest(clientID,atoi(argv[2]),&command,1);

		nomearFifo(clientID,nomeFifo);

		int requestFifo = openFifo(nomeFifo);

		if(writeRequest(requestFifo,request) == -1){
			free(command);
			command = NULL;
			destroyRequest(request);
			return -1; // funcao fecha o fifo para escrita
		} 

		free(command);
		command = NULL;
		destroyRequest(request);
		return 0;
	}
	if(strncasecmp(argv[3],"-p") == 0){

		Command * command = parseMultipleCommands(argv[4]);
		request = createRequest(clientID,atoi(argv[2]),command,countCommands(argv[4]));
		
		nomearFifo(clientID,nomeFifo); // Nomear fifo com id do client
		int fifo = openFifo(nomeFifo); // 

		if(writeRequest(fifo,request) == -1){  // Caso de haver erro a escrever o request no fifo
			free(command);
			command = NULL;
			destroyRequest(request);
			return -1;
		}

		free(command);
		command = NULL;
		destroyRequest(request);
		return 0;
	}

	return -1;
}

int main(int argc,char ** argv){
	switch (argc)
	{
	case EXECUTE:
		/*Request * request = NULL;
		if(strncasecmp(argv[3],"-u",sizeof("-u")) == 0){
			Command command = parseUniqueCommand(argv[4]);
			request = createRequest(clientID++,stringToInt(argv[2]),&command,1);
			lseek(idfd,0,SEEK_SET);
			if(write(idfd,&clientID,sizeof(int)) == -1) perror("ID request não atualizado");
			close(idfd);
			
			destroyRequest(request);
			return 0;
		}
		else{
			Command * commands = parseMultipleCommands(argv[4]);
			request = createRequest(clientID++,stringToInt(argv[2]),commands,countCommands(argv[4]));
			lseek(idfd,0,SEEK_SET);
			if(write(idfd,&clientID,sizeof(int)) == -1) perror("ID request não atualizado");
			close(idfd);
			// sendRequest to fifo
			destroyRequest(request);
			return 0;
		}*/
		int id = getClientId();

		// Sucesso no envido do request de execução
		if(sendExecuteRequest(id,argc,argv) == -1){
			perror("O request nao foi enviado devidamente");
			updateClientID(id);
			return 1;
		}

		// Sucesso
		printf("Request do client %d foi escrito com sucesso",id);

		// Ler reply

		
		// atualizar o 
		updateClientID(id);
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
