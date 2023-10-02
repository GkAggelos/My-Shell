#include "mysh.h"


//find the tokes of the command
//all the strings separated by space
int parse(char *str, char **parsed) {
    int i;
    char copy_str[MAXBUFF];
    char *tmp = copy_str;

    strcpy(copy_str, str);

    for (i = 0; i < MAXLIST; i++) {

        parsed[i] = strsep(&tmp, " \n\0");
  
        if (parsed[i] == NULL) 
            break;

        if (strlen(parsed[i]) == 0)
            i--;
    }
    return i;
}

//find all the individual commands
void seperateSemi(char *str, char **commands) {
    int i;
    char copy_str[MAXBUFF];
    char *tmp = copy_str;

    strcpy(copy_str, str);

    for (i = 0; i < MAXCOMMANDS; i++) {

        commands[i] = strsep(&tmp, ";\n");
  
        if (commands[i] == NULL)
            break;

        if (strlen(commands[i]) == 0)
            i--;
    }
}