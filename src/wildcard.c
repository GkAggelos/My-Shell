#include "mysh.h"

//find the file subset that determine str
int wildcard(char files[][MAXSIZEOFWORD],char *str) {

    glob_t globbuf;
    int n;

    if ( (n = glob(str, GLOB_NOSORT, NULL, &globbuf)) == 0 ) {
        int number = globbuf.gl_pathc;
        for (size_t k = 0; k < globbuf.gl_pathc; k++) {
            strcpy(files[k], globbuf.gl_pathv[k]);
            files[k][strlen(globbuf.gl_pathv[k])] = '\0';
        }
        globfree(&globbuf);
        return number;
    }
    else if (n ==  GLOB_NOSPACE) {
        printf("Running out of memory\n");
        return -1;
    }
    else if (n == GLOB_ABORTED) {
        printf("Read error\n");
        return -1;
    }
    else if (n == GLOB_NOMATCH) {
        printf("No found matches\n");
        return -1;
    }
    else {
        perror("glob");
        return -1;
    }
}