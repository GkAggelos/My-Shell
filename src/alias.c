#include "mysh.h"

//print all aliases
void printalias(struct aliasEntry **a, int count) {
    for (int i = 0; i < count; i++) {
        printf("Alias: %s Command: %s\n", a[i]->alias, a[i]->command);
    }
    
}

//find the position of the alias
int inAliasList(struct aliasEntry **aliasList, char *alias, int count) {

    for (int i = 0; i < count; i++) {

        if ( strcmp(aliasList[i]->alias, alias) == 0 ) 
            return i;
    }
    
    return -1;
}

//save alias in the list
int saveAlias(struct aliasEntry **AliasList, char *name_temp, char *command, int count) {

    int i, flag = 0, j = 0, position;
    char str[MAXBUFF];
    char name[30];

    strcpy(name,name_temp);

    //find the command to save for the alias from "createalias" command
    for (i = 0; i < strlen(command); i++) {
        if (command[i] == '"') {
            if (flag) break;
            flag = 1;
            continue;
        }
        if (flag) {
            str[j] = command[i];
            j++;
        }
    }
    str[j] = '\0';

    //if allready exist replace it
    if ((position = inAliasList(AliasList, name, count)) !=  -1) {
        strcpy(AliasList[position]->command, str);
        return count;
    }

    //else create a new one
    struct aliasEntry *newAlias = malloc(sizeof(struct aliasEntry));
    
    name[strlen(name)] = '\0';
    strcpy(newAlias->alias,name);
    strcpy(newAlias->command,str);
    AliasList[count] = newAlias;

    return ++count;
}

//delete alias from list
int deleteAlias(struct aliasEntry **AliasList, char *name, int count) {
    int position;

    //find it and delete it or there is an error
    if ((position = inAliasList(AliasList, name, count)) !=  -1) {
        free(AliasList[position]);
        
        //fill the gap
        for (int i = position; i < (count - 1); i++) {
            AliasList[i] = AliasList[i + 1];
        }
        
        return --count;
    }
    else {
        printf("No such alias name\n");
        return count;
    }
}