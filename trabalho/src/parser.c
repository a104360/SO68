#include "../include/parser.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



/*static char * concat(int argc,char ** argv, int start,int end){
    if(argc == 0) return NULL;
    if(start < 0) start = 0;
    int totalSize = 1;
    
    if(start == end) totalSize = strlen(argv[start]); 
    else for(int i = start;i < end && i < argc;i++) totalSize += strlen(argv[i]) + 1;

    char * string = malloc(sizeof(char) * totalSize);

    int counter = start;

    for(int i = 0; i < totalSize && counter <= end && counter < argc;){
        for(int j = 0;j < strlen(argv[counter]);j++){
            string[i] = argv[counter][j];
            i++;
        }
        if(i+1 != totalSize) {
            string[i] = ' ';
            i++;
        }
        counter++;
    }
    string[totalSize] = '\0';
    return string;
}*/

// Conta quantos comandos existem no pipeline passado como argumento
int countCommands(const char *argv) {
    if (argv == NULL || argv[0] == '\0')
        return 0;
    int count = 0;
    char *copy = strdup(argv);
    char *saveptr = NULL;
    char *token = strtok_r(copy, "|", &saveptr);
    while (token) {
        count++;
        token = strtok_r(NULL, "|", &saveptr);
    }
    free(copy);
    return count;
}

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

// Separa cada comando do pipeline por ordem 
static char *cmdtok(const char *argv, int start, char *saveptr) {
    if (argv == NULL)
        return NULL;
    char *copy = strdup(argv + start); // Start from the given position
    if (saveptr == NULL)
        return strtok_r(copy, "|", &saveptr);
    saveptr = saveptr + 1;
    return strtok_r(NULL, "|", &saveptr);
}


char * parseUniqueCommand(const char * argv){
    return strdup(argv);
}

char ** parseMultipleCommands(const char * argv){
    if(argv == NULL) return NULL;
    int max = countCommands(argv);
    char ** args = malloc(sizeof(char *) * max);
    char * saveptr = NULL;
    for(int i = 0;i < max;i++){
        args[i] = cmdtok(argv,getIndexNCommand(argv,i),saveptr);
    }
    return args;
}

void getReply(int fd,char * reply){

    lseek(fd,0,SEEK_SET);
    char buffer[BUFFERSIZE];

    int offset = 0;
    int n;

    while((n = read(fd,buffer,BUFFERSIZE))){
        offset += n;
        snprintf(reply+offset,offset,"%s",buffer);
    }
    
}

/*
int stringToInt(char * argv){
    int result = 0;
    int i = 0;

    // Check for negative sign
    int sign = 1;
    if (argv[0] == '-') {
        sign = -1;
        i = 1;
    }

    // Iterate through each character of the string and build the integer
    for (; argv[i] != '\0'; i++) {
        if (argv[i] >= '0' && argv[i] <= '9') {
            result = result * 10 + (argv[i] - '0');
        } else {
            // Invalid character encountered
            printf("Invalid character detected: %c\n", argv[i]);
	    return -123;
        }
    }

    return result * sign;
}
*/