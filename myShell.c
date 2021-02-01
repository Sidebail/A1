#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <string.h>
#include <signal.h>
#include <fcntl.h>

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

void tltrn_RunProcess(char* arguments[], int returnImm, int input, 
    int output, char* fileNameIn, char* fileNameOut, int piped, char* secondArguments[])
{
    /**
     * Sigaction declaration
     */
    struct sigaction sigact;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);

    int pipefd[2];

    sigact.sa_handler = sigquit;
    if (sigaction(SIGQUIT, &sigact, NULL) < 0) {
        perror("sigaction()");
        exit(1);
    }
    
    pid_t childpid;
    int status;
    int file_in;
    int file_out;
    char child_output[1024];
    if(piped)
    {
        if(pipe(pipefd) != -1)
        {
            printf("Pipe Created!\n");
        }
    }
        
    childpid = fork();
    if ( childpid >= 0 ) 
    {/* fork succeeded */
        /*
        *   Child Process
        */
        if ( childpid == 0 ) {   
         
            
            if(input)
            {
               
                file_in = open(fileNameIn, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(file_in, 0);
            }
            if(output)
            {
                printf("%s\n", fileNameOut);
                file_out = open(fileNameOut, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(file_out, 1);
                
            }
            if(piped)
            {
                close(STDOUT_FILENO);
                dup(pipefd[1]);
                close(pipefd[0]);
                close(pipefd[1]);
                
            }

            status = execvp (arguments[0], arguments);
            
            if(input)
                close(file_in);
            if(output)
                close(file_out);
           
            exit(errno);
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
                if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
                {
                    printf("Shell error -- System sent error code %d\n", WEXITSTATUS(status));
                    if(WEXITSTATUS(status) == 2)
                    {
                        printf("Shell error -- No such command found!\n");
                    }
                }
                
            }

            if(piped)
            {
                pid_t pipeChild = fork();
                int pipeStatus;
                if(pipeChild==0)
                {
                    close(STDIN_FILENO);
                    dup(pipefd[0]);
                    close(pipefd[1]);
                    close(pipefd[0]);

                    /**
                    printf("%s\n", secondArguments[0]);
                    int bytesAmount = read(pipefd[0], child_output, sizeof(child_output));
                    int i = 0;
                    for(i = 0; secondArguments[i] != NULL; i++);
                    secondArguments[i] = malloc(sizeof(child_output));
                    memcpy(secondArguments[i], child_output, sizeof(child_output));               
                    secondArguments[i+1] = NULL;
                    //printf("Output: %ld\n", sizeof(secondArguments) / sizeof(secondArguments[0]));
                    */
                    pipeStatus = execvp(secondArguments[0], secondArguments);
                    exit(1);
                }
                else
                {
                    close(pipefd[0]);
                    close(pipefd[1]);
                    waitpid(pipeChild, &pipeStatus, 0);
                }
                
            }

            
            
        }
    } 
    else 
    {
        printf("ERROR OCCURED DURINF FORK!\n");
        perror("fork");
        exit(-1);
    }

}

void interpretCommand(char* arguments[], int returnImm, int input, int output, char* fileNameIn, char* fileNameOut, char* secondArguments[], int piped)
{
    
    if(strcmp(arguments[0], tltrn_kill) == 0)
    {
        tltrnCommand_kill(0);
    }
    else
    {
        
        tltrn_RunProcess(arguments, returnImm, input, output, fileNameIn, fileNameOut, piped, secondArguments);
    }

    
        
}

int main(int argc, char *argv[])
{
    char input[INPUT_MAX - 2];
    char* inputArguments[32];
    char* firstArguments[32];
    //char* secondArguments[32];
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
    int piped = 0;

    char PATHvar[] = "/bin";
    char HISTFILEvar[] = "./CIS3110_history";
    char HOMEvar[] = "./";

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
        fileInput = 0;
        fileOutput = 0;
        piped = 0;
        inArgument = 0;

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

            if(input[i] == '<' && fileSpecified == 0)
            {
                printf("CHECK\n");
                fileSpecified = 1;
                fileInput = 1;
                fileNameIn = malloc((strlen(input) - i) * sizeof(char));
                if(input[i+1] == ' ')
                {
                    memcpy(fileNameIn, &input[i+2], strlen(input) - i);
                    printf("|%s|\n", fileNameIn);
                }
                else
                {
                    memcpy(fileNameIn, &input[i+1], strlen(input) - i);
                    printf("|%s|\n", fileNameIn);
                }

            }

            if(input[i] == '|' && fileSpecified == 0)
            {
                piped = 1;
                for(int i = 0; i < argumentsAmount; i++)
                {
                    firstArguments[i] = malloc(strlen(inputArguments[i]));
                    strcpy(firstArguments[i], inputArguments[i]);
                    memset(inputArguments[i],'\0',strlen(inputArguments[i]));
                }
                firstArguments[argumentsAmount] = NULL;

                argumentsAmount = 0;
                continue;
                

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
            
            if(piped == 0)
            {
                interpretCommand(inputArguments, returnImm, fileInput, fileOutput, fileNameIn, fileNameOut, NULL, piped);
            }
            else
            {
                interpretCommand(firstArguments, returnImm, fileInput, fileOutput, fileNameIn, fileNameOut, inputArguments, piped);
            }
            
                    
        }
               
    }    
    return 0;
}

