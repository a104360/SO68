#define EXECUTE 5
#define STATUS 2

typedef char * Command;

/// @brief Struct para definir um pedido do cliente
typedef struct request Request;

/// @brief Construtor de pedido
/// @param  id id do pedido
/// @param  time referencia temporal para a resposta do servidor
/// @param  argv array de argumentos passados ao cliente pelo utilizador
/// @param  argc numero de elementos do array de argumentos
/// @return Pointer para o request criado
Request *createRequest(int, int,Command *, int);

/// @brief Getter id pedido
/// @param  request Pointer para o request
/// @return id do pedido
int getRid(Request *);

/// @brief Getter tempo de referencia do pedido
/// @param  request Pointer para o request
/// @return Tempo de referencia em milisegundos
int getRtime(Request *);

/// @brief Getter dos comandos
/// @param  request Pointer para o request
/// @return Array com os comandos a executar
char ** getCommands(Request *);

/// @brief Getter numero de comandos
/// @param  request Pointer para o request
/// @return Número de comandos a executar no pedido
int getNCommands(Request *);

/// @brief Função de desalocação de memória para o request
/// @param  request Pointer para o request
void destroyRequest(Request *);

/// @brief Função para enviar o request para um fifo
/// @param  fifo File descriptor do fifo para o qual o Request vai ser escrito
/// @param  request Pointer para o Request a ser escrito no fifo
/// @return 0 -> sucesso || -1 -> erro
int writeRequest(int,Request *);