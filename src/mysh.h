#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>
#include <glob.h>

#define MAXBUFF 1024
#define MAXLIST 100
#define MAXCOMMANDS 20
#define MAXSIZEOFWORD 100
#define READ 0
#define WRITE 1

struct aliasEntry {
    char alias[MAXSIZEOFWORD];
    char command[MAXBUFF];
};

int saveAlias(struct aliasEntry **, char *, char *, int);
int deleteAlias(struct aliasEntry **, char *, int);
void printalias(struct aliasEntry **, int);
int parse(char *, char **);
void seperateSemi(char *, char **);
int wildcard(char [][MAXSIZEOFWORD],char *);
void printHistory(char [][MAXBUFF], int);
int execute_command(char *, struct aliasEntry **, int *, char [][MAXBUFF], int);
void background_termination(int);
int inAliasList(struct aliasEntry **, char *, int);