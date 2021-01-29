#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <string.h>
#include <signal.h>

#define INPUT_MAX 128

// LIST OF COMMANDS
char* tltrn_kill = "kill";

void tltrnCommand_kill(int exitCode)
{
    printf("──── SYSTEM SHUTDOWN ────\n\n");
    exit(exitCode);
}

void sigquit(int signo) {
    exit(0);
}

void tltrn_RunProcess(char command[], char* arguments[], int returnImm)
{
    /**
     * Sigaction declaration
     */
    struct sigaction sigact;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);

    sigact.sa_handler = sigquit;
    if (sigaction(SIGQUIT, &sigact, NULL) < 0) {
        perror("sigaction()");
        exit(1);
    }
    
    pid_t childpid;
    int status;
    childpid = fork();
    if ( childpid >= 0 ) 
    {/* fork succeeded */
        /*
        *   Child Process
        */
        if ( childpid == 0 ) {   
         
            
            status = execvp (command, arguments);
            
            exit(status);
        } 
        else 
        {   
            /* 
             *Parent Process
             */        
            sigact.sa_handler = SIG_DFL;
            sigaction(SIGQUIT, &sigact, NULL); 
            if(returnImm == 0)
            {
                waitpid(childpid,&status,0);
            }
            else
            {
                /* pid holds the id of child */
                //sleep(1); /* pause for 1 secs */
                kill(childpid, SIGQUIT);
            }
            
        }
    } 
    else 
    {
      perror("fork");
      exit(-1);
    }

}

void interpretCommand(char input[], char* arguments[], int returnImm)
{
    
    if(strcmp(input, tltrn_kill) == 0)
    {
        tltrnCommand_kill(0);
    }
    else
    {
        
        tltrn_RunProcess(input, arguments, returnImm);
    }

    
        
}

int main(int argc, char *argv[])
{
    char input[INPUT_MAX - 2];
    char cmd[INPUT_MAX - 2];
    char* inputArguments[32];
    char currentArgument[INPUT_MAX - 2];
    int currentEntryIndex = 0;
    int argumentsAmount = 0;
    int returnImm = 0;

    /**
     * Sigaction declaration and initialization
     */ 

    printf("──────────▄▄▄▄▄▄▄▄▄──────────\n");
    printf("───────▄█████████████▄───────\n");
    printf("▐███▌─█████████████████─▐███▌\n");
    printf("─████▄─▀███▄─────▄███▀─▄████─\n");
    printf("─▐█████▄─▀███▄─▄███▀─▄█████▌─\n");
    printf("──▐█▀█▄▀███─▄─▀─▄─███▀▄█▀█▌──\n");
    printf("───██▄▀█▄██─██▄██─██▄█▀▄██───\n");
    printf("────▀██▄▀██─█████─██▀▄██▀────\n");
    printf("───▄──▀████─█████─████▀──▄───\n");
    printf("───██────────███────────██───\n");
    printf("───██▄────▄█─███─█▄────▄██───\n");
    printf("───████─▄███─███─███▄─████───\n");
    printf("───████─████─███─████─████───\n");
    printf("───████─████─███─████─████───\n");
    printf("───████─████▄▄▄▄▄████─████───\n");
    printf("───▀███─█████████████─███▀───\n");
    printf("─────▀█─███─▄▄▄▄▄─███─█▀─────\n");
    printf("────────▀█▌▐█████▌▐█▀────────\n");
    printf("───────────███████───────────\n");
    printf("──────────TELETRAAN──────────\n");
    printf("──────────█ver 1.0█──────────\n");
    printf("────WELCOME, CYBERTRONIAN────\n\n");

    while(strcmp(input, "kill") != 0)
    {
        strcpy(cmd, "");
        currentEntryIndex = 0;
        argumentsAmount = 0;
        returnImm = 0;

        printf ("cybertronian@tltrn:>> ");
        fgets(input, INPUT_MAX, stdin);
        input[strlen(input) - 1] = 0;
        for(int i = 0; i < strlen(input) - 1; i++)
        {
            if(input[i] == ' ')
            {
                if(currentEntryIndex == 0)
                {
                    
                    memcpy(cmd, &input[currentEntryIndex], i - currentEntryIndex);
                    inputArguments[0] = malloc(strlen(cmd)*sizeof(char));
                    memcpy(inputArguments[0], cmd, strlen(cmd));
                    currentEntryIndex = i + 1;
                    argumentsAmount++;

                }
                else if(i != strlen(input) - 2 && input[i+1] != ' ')
                {
                    inputArguments[argumentsAmount] = malloc((i - currentEntryIndex) * sizeof(char));;
                    memcpy(inputArguments[argumentsAmount], &input[currentEntryIndex], i - currentEntryIndex);
                    currentEntryIndex = i + 1;
                    argumentsAmount++;
                    
                }
                
            }            
        }

       
        if(currentEntryIndex == 0)
        {
            
            strcpy(cmd, input);
            inputArguments[0] = malloc(strlen(cmd)*sizeof(char));
            
            memcpy(inputArguments[0], cmd, strlen(cmd));
            
            
        }
        else if (input[strlen(input) - 1] != ' ')
        {
            inputArguments[argumentsAmount] = malloc(strlen(currentArgument)*sizeof(char));
            memcpy(currentArgument, &input[currentEntryIndex], strlen(input) - 1);
            
            if(input[strlen(input) - 1] != '&')
            {
                returnImm = 0;
                memcpy(inputArguments[argumentsAmount], &input[currentEntryIndex], strlen(input) - currentEntryIndex);
                printf("%s", inputArguments[argumentsAmount]);
            }
            else
            {
                
                returnImm = 1;
            }
            
                
            
        }
        
        
        inputArguments[argumentsAmount+1] = NULL;
        
        interpretCommand(cmd, inputArguments, returnImm);
    }    
    return 0;
}

