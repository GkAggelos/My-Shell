#include "mysh.h"

int main(void) {

    struct aliasEntry *aliasList[30];
    int aliasCount = 0;
    int exit = 0;
    char buff[MAXBUFF];
    int countHistory = 0, flag = 0;
    int next_position = 0;
    char History[20][MAXBUFF];
    static struct sigaction act1, act2;

    //ctrl c and ctrl z signal handle
    //and backgroung child termination handle 
    act1.sa_handler = SIG_IGN;
    sigfillset(&(act1.sa_mask));
    act2.sa_handler = background_termination;
    sigfillset(&(act2.sa_mask));
    act2.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &act2, NULL);
    sigaction(SIGINT, &act1, NULL);
    sigaction(SIGTSTP, &act1, NULL);
    
    while(!exit){

        printf("in-mysh-now:>");

        //read command
        if (fgets(buff,MAXBUFF,stdin) == NULL) {
            perror("command read error");
            continue;
        }

        //save command at history   
        strcpy(History[next_position], buff);
        History[next_position][strlen(buff)] = '\0';
        next_position++;

        if (!flag) {
            countHistory++;
        }

        if (next_position == 20) {
            next_position = 0;
            flag = 1;
        }

        //execute command
        exit = execute_command(buff, aliasList, &aliasCount, History, countHistory);    
    }

    //free all aliases that was made
    for (int i = 0; i < aliasCount; i++) {
        free(aliasList[i]);
    }
    
    return 0;
}