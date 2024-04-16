#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char * concat(int argc,char ** argv, int start,int end){
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
}

static int getCommandEnd(int argc, char ** argv,int start){
    int i = start;
    while(argv[i] != "|" && i < argc){
        i++;
    }
    return i;
}

char * parseCommand(int argc,char ** argv,int index){
    int last = getCommandEnd(argc,argv,index);
   char * command = NULL;
}
/*
int main()
{
    int start = -2;
    int end = 10000;
    int len = 5;
    char * argv[] = {"Tu","és","um","coninhas","!"};
    char * n = concat(len,argv,start,end);
    
    printf("%s\n",n);
    
    free(n);

    return 0;
}*/

/*  Casos de teste

int main()
{   
    int start = 1;
    int end = 3;
    int len = 5;
    char * argv[] = {"program", "This", "is", "a", "test"};
    char * n = concat(len,argv,start,end);
    
    int argc = 4;
    int start = 0;
    int end = argc-1;
    char * argv[] = {"program", "This", "is", "a", "test"};
    char * n = concat(argc,argv,start,end);
    
    int argc = 3;
    int start = 1;
    int end = 5;
    char * argv[] = {"program", "This", "is", "a", "test"};
    char * n = concat(argc,argv,start,end);
    
    int argc = 0;
    int start = 0;
    int end = 0;
    char * argv[] = {};
    char * n = concat(argc,argv,start,end);
    
    int argc = 2;
    int start = 1;
    int end = 1;
    char * argv[] = {"program", "Hello"};
    char * n = concat(argc,argv,start,end);
    
    printf("%s\n",n);
    
    free(n);

    return 0;
}
*/
/*

static int howManyCommands(const char * argv){
    if(argv == NULL || argv[0] == '\0') return 0;
    int count = 0;
    char * copy = strdup(argv);
    char * saveptr = NULL;
    char * token = strtok_r(copy,"|\0\n",&saveptr);
    while(token){
        count++;
        token = strtok_r(NULL,"|\0\n",&saveptr);
    }
    return count;
}

// Obter o comando na posicao n do pipeline de comandos a executar
// NOTA: caso a ordem posicao do comando do pipeline ultrapassar 
// o numero de elementos, a funcao devolve o primeiro comando do pipeline
static int getCommandN(char * argv,int n){
    if(n == 0) return 0;
    int size = strlen(argv);
    int count = 0;
    int i = 0;
    for(;i < size && count != n && argv[i] != '\0';i++){
        if(argv[i] == '|') count++;
    }
    if(i != '\0') return i + 1;
    return 0;
}

static char * cmdtok(char * argv,int start,char * saveptr){
    if(argv == NULL) return NULL; 
    char * copy = strdup(argv);
    if(saveptr == NULL)
        return strtok_r(&copy[start],"|\0\n",&saveptr);
    saveptr = saveptr + 1;
    return strtok_r(NULL,"|\0\n",&saveptr);
}
*/

static int howManyCommands(const char *argv) {
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
    free(copy); // Free dynamically allocated memory
    return count;
}

static int getCommandN(char *argv, int n) {
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

static char *cmdtok(char *argv, int start, char *saveptr) {
    if (argv == NULL)
        return NULL;
    char *copy = strdup(argv + start); // Start from the given position
    if (saveptr == NULL)
        return strtok_r(copy, "|", &saveptr);
    saveptr = saveptr + 1;
    return strtok_r(NULL, "|", &saveptr);
}

int main(int argc, char **argv) {
    printf("Nº argumentos : %d\n", argc);
    if (argc >= 4) { // Check if argc is large enough
        int max = howManyCommands(argv[3]);
        printf("%d\n\n", max);
        char *saveptr = NULL;
        for (int i = 0; i < max; i++) {
            char *token = cmdtok(argv[3], getCommandN(argv[3], i), saveptr);
            if (token != NULL) {
                printf("comando %d : %s\n", i + 1, token);
                free(token); // Free dynamically allocated memory
            }
        }
    }
    if (argc == 3) {
        printf("%s\n", argv[3]); // Accessing argv[3] without checking argc
    }
    return 0;
}