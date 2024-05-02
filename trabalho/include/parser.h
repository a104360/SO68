#ifndef PARSER_H
#define PARSER_H


#define EXECUTE 5
#define STATUS 2

#define BUFFERSIZE 128

typedef char * Command;
typedef Command * Query;



/// @brief Adaptação da strtok para separar os argumentos de um comando
/// @param  cmd Pointer para um comando passado a um 
/// @return Pointer para um array de comandos (Query) para ser passada a um execvp
Query cmdTok(const char *);

/// @brief Parse de um pipe de comandos de qualquer tamanho
/// @param  cmdPipe Argumento passado pelo utilizador 
/// @return Pointer para um array de queries
Query * parsePipe(const char *); 

/// @brief Conta quantos comandos existem no argumento passado 
/// ao cliente
/// @param  argv Argumento com comandos passado ao cliente pelo utilizador
/// @return Nº de comandos presentes no argumento
int countCommands(const char *);

/// @brief Printa a query no ecrâ
/// @param  Query Pointer para array de comandos
void printQuery(Query);


/// @brief Liberta memória detida por uma query
/// @param  Query Pointer para o array de comandos a ser libertados
void freeQuery(Query);

#endif