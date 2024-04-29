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
/// @note O valor do argc não deve incluir o NULL
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
Request * readRequest(const char *,int);
