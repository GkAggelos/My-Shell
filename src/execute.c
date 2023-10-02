#include "mysh.h"

int processString(char** parsed, int start, int pipeInfd) {
    char *inputname, *outputname, *appendname;
    char str[MAXSIZEOFWORD];
    char *program[MAXLIST];
    char files[MAXLIST][MAXSIZEOFWORD];
    bool flagInput = false, flagOutput = false, flagAppend = false;
    int i, hasInput = 0, hasOutput = 0, hasAppend = 0, hasPipe = 0, j = 0;
    int readfd, writefd, appendfd;
    int pipefd[2];
    int background = 0, status, id;

    //for all the tokens from parsed
    for (i = start; i < MAXLIST; i++) {

        //check if it is the last one
        if (parsed[i] == NULL) {
            program[j] = NULL;
            break;
        }

        //check if it is a file name for redirection
        if (flagInput) {
            hasInput = 1;
            inputname = parsed[i];
            flagInput = false;
            continue;
        }
        else if (flagOutput) {
            hasOutput = 1;
            outputname = parsed[i];
            flagOutput = false;
            continue;
        }
        else if (flagAppend) {
            hasAppend = 1;
            appendname = parsed[i];
            flagAppend = false;
            continue;
        }

        strcpy(str,parsed[i]);
        str[strlen(parsed[i])] = '\0';
        
        //check if it is a symbol that indicates redirection 
        //so the next token is a filename
        if ((strcmp(str, ">")) == 0) {
            flagOutput = true;
        }
        else if ((strcmp(str, ">>")) == 0) {
            flagAppend = true;
        }
        else if ((strcmp(str, "<")) == 0) {
            flagInput = true;
        }
        //check if it is a symbol that indicates pipeline 
        //so have to create a pipe
        else if ((strcmp(str, "|")) == 0) {
            if ( pipe(pipefd) == -1 ) {
                perror("pipe"); 
                exit(1);
            }
            hasPipe = 1;
            program[j] = NULL;
            break;
        }
        //check if it has to run at background
        else if ((strcmp(str, "&")) == 0) {
            background = 1;
        }
        //check if it has wildcards 
        //so have to find the file subset
        else if ( (strchr(str, '*') != NULL) || (strchr(str, '?') != NULL) || (strchr(str, '[') != NULL) ) {
            int number = wildcard(files, str);
            if (number > 0) {
                for (int k = 0; k < number; k++) {
                    program[j] = files[k];
                    j++;
                }
            }
        }
        //or it is an argument for the program
        else {
            program[j] = parsed[i];
            j++;
        }
    }

    int pid = fork();

    if (pid == -1) {
        perror("fork");
    } 
    else if (pid == 0) {

        //do not ignore ctrl c and ctrl z signal 
        //if it is not a background process
        //and restore the defult action for SIGCHLD 
        static struct sigaction act;
        act.sa_handler = SIG_DFL;
        sigfillset(&(act.sa_mask));   
        sigaction(SIGCHLD, &act, NULL);

        if (!background) {
            sigaction(SIGINT, &act, NULL);
            sigaction(SIGTSTP, &act, NULL);
        }

        if (hasInput) { //if '<' char was found in string inputted by user
            if ( (readfd = open(inputname, O_RDONLY)) < 0) {
                perror("can't open to read \n");
            }
            dup2(readfd, STDIN_FILENO);
            close(readfd);
        }

        if (hasOutput) { //if '>' was found in string inputted by user
            if ( (writefd = open(outputname, O_WRONLY|O_CREAT, 0666)) < 0) {
                perror("can't open to write \n");
            }
            dup2(writefd, STDOUT_FILENO);
            close(writefd);
        }

        if (hasAppend) { //if '>>' was found in string inputted by user
            if ( (appendfd = open(appendname, O_WRONLY|O_APPEND)) < 0) {
                perror("can't open to write append \n");
            }
            dup2(appendfd, STDOUT_FILENO);
            close(appendfd);
        }

        if (pipeInfd != -1) { //if it is a program from the read end of a pipe
            dup2(pipeInfd, STDIN_FILENO);
            close(pipeInfd);
        }

        if (hasPipe) { //if it is a program from the write end of a pipe
            dup2(pipefd[WRITE], STDOUT_FILENO);
            close(pipefd[WRITE]);
            close(pipefd[READ]);
        }

        if (background) { //if it is a program that has to be executed in background
            printf("proccess with id %d at background\n", getpid());
        }
        execvp(program[0], program);
        perror("execvp");
        return -1;
    } 
    else {
        //execute and the other half of the pipe  
        if (hasPipe) {
            close(pipefd[WRITE]);
            processString(parsed, i + 1, pipefd[READ]);
        }
        
        if (pipeInfd != -1) close(pipeInfd);
        
        //wait for the child to finish
        if (!background) {

            id = waitpid(pid, &status, WUNTRACED);
        }
    }
}

int execute_command(char *buff, struct aliasEntry **aliasList, int *aliasCount, char History[][MAXBUFF], int countHistory) {

    char *parsed[MAXLIST], *commands[MAXCOMMANDS];
    int i;
    char *execute;
    int position;

    //seperate the individual commands
    seperateSemi(buff,commands);

    //for each command
    for (i = 0; i < MAXCOMMANDS; i++) {

        if(commands[i] == NULL) break;
        
        //find the tokens
        parse(commands[i],parsed);

        //erro if no tokens
        if (parsed[0] == NULL) {
            printf("error bad command\n");
            break;
        }

        //if it is an alias run the command of the alias 
        if ( (position = inAliasList(aliasList, parsed[0], *aliasCount)) !=  -1) {
            parse(aliasList[position]->command,parsed);
        }

        //exit
        if ((strcmp(parsed[0], "exit")) == 0) { 
            return 1;
            break;
        }

        //change directory
        if ((strcmp(parsed[0], "cd")) == 0) {
            if (parsed[1] == NULL) {
                printf("error bad command\n");
                break;
            }
            if (chdir(parsed[1]) != 0) {
                perror("chdir failed");
            }
            break;
        }

        //save an alias
        if ((strcmp(parsed[0], "createalias")) == 0) {
            if (parsed[1] == NULL) {
                printf("error bad command\n");
                break;
            }
            if (parsed[2] == NULL) {
                printf("error bad command\n");
                break;
            }
            *aliasCount = saveAlias(aliasList, parsed[1], buff, *aliasCount);
            break;
        }
        if ((strcmp(parsed[0], "printalias")) == 0) {
            printalias(aliasList, *aliasCount);
            break;
        }
        //delete an alias
        if ((strcmp(parsed[0], "destroyalias")) == 0) {
            if (parsed[1] == NULL) {
                printf("error bad command\n");
                break;
            }
            *aliasCount = deleteAlias(aliasList, parsed[1], *aliasCount);
            break;
        } 

        //show history or run a previous command
        if ((strcmp(parsed[0], "history")) == 0) {
            if (parsed[1] == NULL) {
                printHistory(History, countHistory);
                break;
            }
            else {
                int number = atoi(parsed[1]);
                return execute_command(History[number], aliasList, aliasCount, History, countHistory);
            }
        }

        processString(parsed,0,-1);
    }

    return 0;
}

//handle the SIGCHLD signal
//this is for waiting background processes
void background_termination(int signo) {
    int id;
    int status;

    id = waitpid(-1, &status, WNOHANG);

}