#ifndef REQUEST_H
#define REQUEST_H

#include "parser.h"

#define EXECUTE 5
#define STATUS 2
#define REQUESTMAXSIZE 300

/// @brief Struct para definir um pedido do cliente
typedef struct request Request;

/// @brief Construtor de pedido
/// @param  id id do pedido
/// @param  time referencia temporal para a resposta do servidor
/// @param  argv array de argumentos passados ao cliente pelo utilizador
/// @note O valor do argc não deve incluir o NULL
/// @return Pointer para o request criado
Request *createRequest(int, int,Command);

/// @brief Função de desalocação de memória para o request
/// @param  request Pointer para o request
void destroyRequest(void *);





/// @brief Setter id do pedido
/// @param  request Pointer para o request
/// @param  id Id a ser inserido
void setRid(Request *,int);

/// @brief Setter tempo do pedido
/// @param  request Pointer para o request 
/// @param  time Tempo
void setRtime(Request *, int);

/// @brief Getter id pedido
/// @param  request Pointer para o request
/// @return id do pedido
int getRid(Request *);

/// @brief Getter tempo de referencia do pedido
/// @param  request Pointer para o request
/// @return Tempo de referencia em milisegundos
int getRtime(Request *);


/// @brief Getter comando enviado no request
/// @param  request Pointer para o request
/// @return Cópia do comando guardado no request
char * getRCommand(Request *);




/// @brief Printa o Request no standard output
/// @param  request Pointer para o Request a ser impresso
void printRequest(Request *);

/// @brief Função para enviar o request para um fifo
/// @param  fifo Nome do ficheiro para o qual vai ser escrito o Request
/// @param  request Pointer para o Request a ser escrito no fifo
/// @return 0 -> sucesso || -1 -> erro
int writeRequest(const char *,Request *);

/// @brief Obtém o request armazenado no ficheiro, num index arbitrário
/// @param  filename Nome do ficheiro de onde se vai ler o request
/// @param  index Posicao no ficheiro de onde se vai ler o request
/// @return Pointer para o request armazenado no ficheiro
/// @note Se o index for -1, não é ignorado as posição onde se vai ler
Request * readRequest(const char *,int);

/// @brief Copia o request para um novo pointer
/// @param  request Pointer para request 
/// @return  p Pointer para onde vai ser copiado o request
void * copyRequest(Request *);

/// @brief Compara os tempos dos requests
/// @param  r1 Pointer para um request
/// @param  r2 Pointer para outro request
/// @return r1 > r2 -> 1  |  r1 == r2 -> 0  |  r1 < r2 -> -1
int compareRequest(void *,void *);

#endif