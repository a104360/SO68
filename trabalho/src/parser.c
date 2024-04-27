#include "../include/parser.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Apresenta o comando na posicao N da pipeline de comandos
static int getIndexNCommand(const char *argv, int n) {
    if (n == 0)
        return 0;
    int size = strlen(argv);
    int count = 0;
    int i = 0;
    for (; i < size && count != n && argv[i] != '\0'; i++) {
        if (argv[i] == '|')
            count++;
    }
    if (argv[i] == '\0') // Correct the condition to check for end of string
        return 0;
    return i + 1;
}

/// @brief Conta quantos comandos há num pipeline de comandos 
/// @param cmd Pointer para o pipeline de comandos
/// @return Número de comandos existentes no pipeline de comandos
static int countCommands(const char * cmd){
    if(!cmd || cmd[0] == '\0') return 0;
    int count = 1;
    for(int i = 0;cmd[i] != '\0';i++){
        if(cmd[i] == '|' && cmd[i+1] >= ' ' && cmd[i+1] <= '~') 
            count++;
    }
    return count;
}

/// @brief Conta quantos tokens existem num comando
/// @param cmd Pointer para o comando 
/// @return Número de tokens no comando
static int countTokens(const char * cmd){
    if(!cmd || cmd[0] == '\0') return 0;
    int count = 1;
    short first = 0;
    for(int i = 0;cmd[i] != '\0' && cmd[i] != '|' && cmd[i+1] != '|';i++){
        if(cmd[i] == ' '){
            if(first == 0) continue;
            if(cmd[i+1] > ' ' && cmd[i+1] <= '~')
                count++;
            continue;
        }
        if(first == 0){
            first = 1;
        }
    }
    return count;
}

/// @brief Criar tokens para cada argumento 
/// @param cmd Pointer para o comando a ser separado em tokens
/// @return Array de tokens do comando, terminando em NULL
static char ** cmdTok(const char * cmd){
    if(!cmd || cmd[0] == '\0') return NULL;
    char * copy = strdup(cmd);
    char * token = NULL;
    char * saveptr = copy;
    
    int size = countTokens(cmd) + 1;
    
    char ** arraycmds = malloc(sizeof(char *) * size);
    
    token = strtok_r(copy," |\0",&saveptr);
    
    arraycmds[0] = strdup(token);
    
    for(int i = 1;i < size-1;i++){
        token = strtok_r(NULL," |\0",&saveptr);
        if(token == NULL && i < size - 1) token = strtok_r(NULL," |\0",&saveptr);
        arraycmds[i] = strdup(token);
    }
    
    if(copy) free(copy);
    arraycmds[size - 1] = NULL;
    return arraycmds;
}

/// @brief Fazer parse a um pipeline de comandos
/// @param cmdPipe Pointer para a String com o pipeline de comandos
/// @return Pointer para array de comandos 
static char *** parsePipe(const char * cmdPipe){
    if(!cmdPipe || (cmdPipe[0] < ' ' || cmdPipe[0] > '~')) return NULL;
    
    char * cmdPipeCopy = strdup(cmdPipe);
    char * cmdPipeToken = NULL;
    char * saveptr = cmdPipeCopy;
    int parseArraySize = countCommands(cmdPipe) + 1; // Determinar quantos comandos vao ser executados
    
    char *** parseArray = malloc(sizeof(char **) * parseArraySize);
    
    cmdPipeToken = strtok_r(cmdPipeCopy,"|\0\n",&saveptr);

     parseArray[0] = cmdTok(cmdPipeToken);
    
    // Ciclo itera por quantos comandos houver
    for(int i = 1;i < parseArraySize;i++){
        cmdPipeToken = strtok_r(NULL,"|\0\n",&saveptr);
        parseArray[i] = cmdTok(cmdPipeToken);
    }
    if(cmdPipeCopy) free(cmdPipeCopy);
    parseArray[parseArraySize - 1] = NULL;
    return parseArray;
}

/// @brief Printar as informações relativas a um comando
/// @param cmd 
static void printCmds(char ** cmd){
    int i = 0;
    while(cmd[i] != NULL){
        printf("%s ",cmd[i]);
        i++;
    }
    putchar('\n');
}

/// @brief Liberta a memória associada a um comando em array
/// @param tok Pointer para um array de comandos
static void freeCmdTok(char ** tok) {
    int i = 0;
    while(tok[i]){
        free(tok[i]);
        i++;
    }
    free(tok);
}

// Corrigida e testada
void getReply(int fd, char *reply) {
    lseek(fd, 0, SEEK_SET);
    char chunk[128 + 1];
    int bytesRead;
    int totalBytesRead = 0;

    
    while ((bytesRead = read(fd, chunk, 128)) > 0) {
        // Terminar o chunk com \0
        chunk[bytesRead] = '\0';

        // Copiar o chunk para o Array de reposta
        strcpy(reply + totalBytesRead, chunk);

        // Atualizar o total de bytes lidos
        totalBytesRead += bytesRead;
    }

    if (bytesRead == -1) {
        perror("Erro a ler o descritor de ficheiro");
    }
}