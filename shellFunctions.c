#define _XOPEN_SOURCE 700 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>

//PURPOSE: a default error case for if a command could not execute (execvp returns -1)
//It's more readable this way (I like it), but to be fair,
//I really didn't have to make this a function()
void errorExecvp()
{
    perror("> Shell: command not found");
}

//PURPOSE: checks if there is a '<', '>', or '|' symbol
//is basically a boolean type function with four values
//will return 0 if none of those characters are found
//PRE-CONDITIONS: tokens contains values and is Not NULL
int foundChar (char ** tokens, int args)
{
    int i, foundChar = 0, j;
    
    for(i=0; i < args; i++)
    {
    
        for(j=0; j < strlen(tokens[i]); j++)
        {
            if(tokens[i][j] == '<')
            {
                foundChar = 1;
            }
            else if(tokens[i][j] == '>')
            {
                foundChar = 2;
            }
            else if(tokens[i][j] == '|')
            {
                foundChar = 3;
            }
        }
    }
    return foundChar;
}


//PURPOSE: handles most built-in commands (so far, and including these symbols '<', '>', '&')
//if it's built in, it should still work with this function
//as well as having the parent process wait for its child processes that have been forked
//Arguments: the double char array for string arguments 'tokens', ending character
//number of string arguments 'args', and the child process
//PRE-CONDITIONS: 'tokens' has ALREADY been deliminated into string arguments
void builtIns(char ** tokens, pid_t child, char end, int args)
{
    FILE * fp;
    int id[10] = {0}, childProcessNum = 0, status, i;

    child = fork();
    if(child == 0)
    {
        if(end == '&')
        {
            for(i=0; i < childProcessNum; i++)
                id[i+1] = id[i];
            id[childProcessNum] = getpid();
            childProcessNum++;
        }
        switch(foundChar(tokens, args)) //deals with if there is a '<', '>', or '|' in tokens
        {
            case 0:
            {
                if (execvp(tokens[0], tokens) < 0)
                    errorExecvp();
                break;
            }
            case 1:
            {
                tokens[args-2] = NULL;
                fp = freopen(tokens[args-1], "r", stdin);
                if (execvp(tokens[0], tokens) < 0)
                    errorExecvp();

                fclose(fp);
                exit(0);
                break;
            } 
            case 2:
            {
                tokens[args-2] = NULL;
                fp = freopen(tokens[args-1], "w", stdout);
                if (execvp(tokens[0], tokens) < 0)
                    errorExecvp();
                    
                fclose(fp);
                exit(0);
                break;
            }
            default:
                break;
        }
    }
    else if(child > 0)
    {
        if(end != '&')
            waitpid(child, &status, 0);
        
        else
        {
            for(i = childProcessNum; i > 0; i--)
                kill(id[i], SIGKILL);
        }
    }
    else if(child < 0)
    {
        perror("> Shell error (fatal): could not fork\n");
        exit(0);
    }

}

//PURPOSE: exits the shell, this exitShell() is called if "exit" is typed
//Arguments: child process, and arguments for the command to be run
//Pre-conditions: that the child has previously been forked once
void exitShell(pid_t child, char ** args)
{
    printf("> exit\n");
    printf("logout\n"); 
            
    if(child >= 0)
    {
        kill(child, SIGKILL); 
        printf("[PROCESS COMPLETED]\n");
        exit(0);
    }
}            

//PURPOSE: separates a line into a char ** based on spaces
//also keeps running until there are no more tokens to be made, and figures out number of tokens
//ARGUMENTS: the char ** to hold the tokens parsed, the entire line that is being 
//parsed and the an 'int *' to store the number of tokens created
//NOTE: only parses based on spaces
void parseLine(char ** tokens, char * buffer, int * args)
{
    int i = 0;
    tokens[0] = strtok(buffer, " \t\r\n");
    if(tokens[0] == NULL)
    {
        return;
    }
    while(tokens[i] != NULL)
    {
        i++;
        tokens[i] = strtok(NULL, " \t\r\n");
    }
    *args = i;
}

//PURPOSE: uses chdir() to change directory
//ARGUMENTS: a double char array with the fileName as the second string
//and the child process
void changeDirec (char ** tokens, pid_t child)
{
    int changeDirec;
    changeDirec = chdir(tokens[1]);
    if(changeDirec == -1)
        perror("> Shell error (non-fatal): could not change directory");
    if(child >= 0)
        kill(child, SIGKILL);
    
}