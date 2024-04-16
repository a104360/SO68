#define EXECUTE 5
#define STATUS 2

#define BUFFERSIZE 128

/// @brief Faz parse de um unico comando
/// @param  argv Comando passado pelo stdin do client
/// @return Pointer para a string do comando do argumento
char * parseUniqueCommand(const char *);

/// @brief Faz parse de multiplos comandos  
/// @param  args String da pipeline de comandos a executar
/// @return Array de comandos a executar
char ** parseMultipleCommands(const char *);

//int stringToInt(char *);

/// @brief Lê a reply do orquestrador para a string passada
/// @param  fd File descriptor do fifo
/// @param  reply Pointer para onde vai ser passado o reply
void getReply(int,char *);

/// @brief Conta quantos comandos existem no argumento passado 
/// ao cliente
/// @param  argv Argumento com comandos passado ao cliente pelo utilizador
/// @return Nº de comandos presentes no argumento
int countCommands(const char *);