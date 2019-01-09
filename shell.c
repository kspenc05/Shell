#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>

void builtIns(char ** tokens, pid_t child, char end, int args);
void exitShell(pid_t child, char ** args);
void changeDirec(char ** tokens, pid_t child);
void parseLine(char * * tokens, char * buffer, int * args);
int isValidCommand(char * buffer);

int main(int argc, char ** argv)
{
    char buffer[50];
    char * * tokens;
    int args, background = 0;
    pid_t child; 
    child = fork();
    
    tokens = malloc(sizeof(char*)*20);
    
    while(strncmp("exit", buffer, 4) != 0)
    {
        background = 0;
        printf("%s> ", getcwd(NULL, 0));
        fgets(buffer, 50, stdin);    
        parseLine(tokens, buffer, &args);
        if(tokens[0] != NULL)
        {
            if(strncmp("exit", tokens[0], 4) == 0)
                break;
            
            if(strncmp("cd", tokens[0], 2) == 0)
            {
                changeDirec(tokens, child);
                continue;
            }
            
            if(args > 0)
            {
                if(strcmp("&", tokens[args-1]) == 0)
                {
                    builtIns(tokens, child, '&', args);
                    background = 1;
                }
            }
            
            if(background == 0)
                builtIns(tokens, child, ' ', args);
        }      
    } 
    exitShell(child, tokens);
    free(tokens);
    return 0;
}