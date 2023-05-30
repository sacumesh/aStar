#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

static int compString (void * s1, void * s2) {
  return strcmp((char*)s1, (char*)s2);
}

/*************************************************************
 * Function to display an element of the list
 * @param s the string to display
 *************************************************************
 */
static void prString(void * s) {
  printf("%s",(char*)s);
}


List * split(char *str, char *seperators, int *count) {
    List * list =  newList(&compString, &prString);
    char *token = strtok(str, seperators);
    while (token != 0) {
        addListAt(list, list->nelts + 1, token);
        token = strtok(0, seperators);
        (*count)++;
    }
    return list;
}

int main() {
    FILE * f;
    size_t sz, len;
    char *line;
    int count = 0;
    List * data;

    f = fopen("FRANCE.MAP", "r");
        while ((len = getline(&line, &sz, f)) != -1)
        {   
            if (len == 1)
                continue;

            for (int i = 0; i < len; i++) {
            if (line[i] == '\n') {
                line[i] = '\0';
                break;
            }
        }
            data = split(line, "\t ", &count);
            displayList(data);
        }
    return 0;
}

