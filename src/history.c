#include "mysh.h"

//print history
void printHistory(char History[][MAXBUFF], int count) {
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i, History[i]);
    }
}