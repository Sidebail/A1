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
char* tltrn_kill = "exit";

void tltrnCommand_kill(int exitCode)
{
    printf("──── SYSTEM SHUTDOWN ────\n\n");
    exit(exitCode);
}

void sigquit(int signo) {
    exit(0);
}

void tltrn_RunProcess(char* arguments[], int returnImm, int input, int output, char* fileNameOut, char* fileNameIn)
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
    FILE* file_in;
    FILE* file_out;
    childpid = fork();
    if ( childpid >= 0 ) 
    {/* fork succeeded */
        /*
        *   Child Process
        */
        if ( childpid == 0 ) {   
         
            printf("%s\n", "Child");
            if(input)
            {
                file_in = freopen(strcat(fileNameIn, ".txt"), "r", stdin);
            }
            if(output)
            {
                file_out = freopen(strcat(fileNameOut, ".txt"), "w+", stdout);
            }
            status = execvp (arguments[0], arguments);
            printf("FILE - , file_out\n");
            fclose(file_in);
            fclose(file_out);
            exit(-1);
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
                kill(childpid, SIGQUIT);
            }
            else
            {
                /* pid holds the id of child */
                //sleep(1); /* pause for 1 secs */
                //kill(childpid, SIGQUIT);
            }
            
        }
    } 
    else 
    {
      perror("fork");
      exit(-1);
    }

}

void interpretCommand(char* arguments[], int returnImm, int input, int output, char* fileNameIn, char* fileNameOut)
{
    
    if(strcmp(arguments[0], tltrn_kill) == 0)
    {
        tltrnCommand_kill(0);
    }
    else
    {
        
        tltrn_RunProcess(arguments, returnImm, input, output, fileNameIn, fileNameOut);
    }

    
        
}

int main(int argc, char *argv[])
{
    char input[INPUT_MAX - 2];
    char* inputArguments[32];
    //char currentArgument[INPUT_MAX - 2];
    char* fileNameOut;
    char* fileNameIn;

    // Booleans
    int currentEntryIndex = 0;
    int argumentsAmount = 0;
    int returnImm = 0;
    int fileSpecified = 0;
    int fileInput = 0;
    int fileOutput = 0;

    int inArgument = 0;

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
        currentEntryIndex = -1;
        argumentsAmount = 0;
        returnImm = 0;
        fileSpecified = 0;

        printf ("cybertronian@tltrn:>> ");
        fgets(input, INPUT_MAX, stdin);
        input[strlen(input) - 1] = 0;

        /**
         * Command parsing algorithm
         * IF LETTER IS NOT ' ' (space)
         * THEN WE ARE IN TEH COMMAND
         *      IF LETTER IS ' ' (space)
         *      THEN WE LEFT THE COMMAND
         *          COPY OVER COMMAND, REWRITE INDEXES!
         * 
         */
        for(int i = 0; i <= strlen(input); i++)
        {
            /**
             * Check for Special character!
             * 
             */ 
            if(input[i] == '>' && fileSpecified == 0)
            {
                printf("CHECK\n");
                fileSpecified = 1;
                fileOutput = 1;
                fileNameOut = malloc((strlen(input) - i) * sizeof(char));
                if(input[i+1] == ' ')
                {
                    memcpy(fileNameOut, &input[i+2], strlen(input) - i);
                    printf("|%s|\n", fileNameOut);
                }
                else
                {
                    memcpy(fileNameOut, &input[i+1], strlen(input) - i);
                    printf("|%s|\n", fileNameOut);
                }

            } 

            /**
             * Check for the argument entry!
             */

            if(input[i] != ' ' && fileSpecified == 0 && input[i] != '\0' )
            {
                printf("InArg\n");
                if(inArgument == 0)
                {
                    currentEntryIndex = i;
                }
                inArgument = 1;
                
            }

            if(input[i] == ' ' && fileSpecified == 0 && inArgument == 1)
            {
                printf("Space\n");
                inputArguments[argumentsAmount] = malloc((i - currentEntryIndex) * sizeof(char));
                memcpy(inputArguments[argumentsAmount], &input[currentEntryIndex], i - currentEntryIndex);
                argumentsAmount++;
                
                inArgument = 0;
            }

            if(input[i] == '\0' && inArgument == 1)
            {
                printf("EndOfStr\n");
                inputArguments[argumentsAmount] = malloc((i - currentEntryIndex) * sizeof(char));
                memcpy(inputArguments[argumentsAmount], &input[currentEntryIndex], i - currentEntryIndex);
                argumentsAmount++;
                inArgument = 0;
            }

        }

    
        if(currentEntryIndex != -1)
        {
             if(strcmp(inputArguments[argumentsAmount-1], "&") == 0)
        {
            printf("Terminator\n");
            returnImm = 1;
            inputArguments[argumentsAmount-1] = NULL;
        }
        else
        {
            //inputArguments[argumentsAmount] = malloc(0);
            inputArguments[argumentsAmount] = NULL;
        }
        
            interpretCommand(inputArguments, returnImm, fileInput, fileOutput, fileNameIn, fileNameOut);
        }
       

        /**
        for(int i=0;i<argumentsAmount;i++)
            printf("|%s|\n", inputArguments[i]);

        for(int i=0;i<argumentsAmount;i++)
            memset(inputArguments[i],'\0',strlen(inputArguments[i]));
        
        memset(temp,'\0',strlen(temp));
        */
        
        
    }    
    return 0;
}

