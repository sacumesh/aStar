#include "Map.h"
#include <stdio.h>
#include <string.h>

/* Maximum length for character arrays */
#define MAX_LENGTH 100

List *createMapFromFile(FILE *file);

int main(int nWords, char *words[]) {
    City *start;
    City *goal;
    Map *m;
    char startStr[MAX_LENGTH];
    char goalStr[MAX_LENGTH];
    FILE *file;

    /* open the file for reading */
    file = fopen("../FRANCE.MAP", "r");

    /* return an error code if the file cannot be opened */
    if (!file)
        return 2;

    /* create a map from the file */
    m = createMapFromFile(file);

    /* close the file */
    fclose(file);

    /* return an error code if the map creation fails */
    if (!m) return 0;

    /* If command line arguments are provided, copy the start and goal city names from the arguments */
    if (nWords >= 3) {
        strcpy(startStr, words[1]);
        strcpy(goalStr, words[2]);
    }
        /* If command line arguments are not provided, prompt the user to enter the start and goal city names */
    else {
        /* Read the start city name from the user */
        printf("Enter start? ");
        scanf("%s", startStr);

        /* Read the goal city name from the user */
        printf("Enter goal? ");
        scanf("%s", goalStr);
    }

    /* find the start city in the map, otherwise return an error code if the start city is not found */
    start = searchCity(m, startStr);
    if (!start)
        return 1;

    /* find the goal city in the map, otherwise return an error code if the goal city is not found  */
    goal = searchCity(m, goalStr);
    if (!goal)
        return 1;

    aStar(m, start, goal);
    delMap(m);

    return 0;
}

/* creates a map from a given file, otherwise return 0 if fails */
Map *createMapFromFile(FILE *file) {
    char *line;
    size_t len;
    char value1[MAX_LENGTH];
    int value2;
    int value3;
    Map *m;
    status st;
    City *c1, *c2;
    Neighbour *n;

    /* create a new map, otherwise return 0 if fails */
    m = newMap();
    if (!m) return 0;

    line = 0;
    c1 = c2 = 0;
    while (getline(&line, &len, file) != -1) {
        int entriesRead = sscanf(line, "%s %d %d", value1, &value2, &value3);
        /* if the entries read is 3 it's a City to add; */
        if (entriesRead == 3) {
            /* check if the city is already in the map */
            c1 = searchCity(m, value1);

            /* if the city is not found create a new city and add it to the map */
            if (!c1) {

                /* Create a new city, otherwise delete the map and return 0 if fails */
                c1 = newCity(value1);
                if (!c1) {
                    delMap(m);
                    return 0;
                }

                /* Add the new city to the map, otherwise delete the map and return 0 if fails */
                st = addCity(m, c1);
                if (st) {
                    delMap(m);
                    fprintf(stderr, "%s\n", message(st));
                    return 0;
                }

            }
            c1->lat = value2;
            c1->lng = value3;
        }
            /* if the entries read is two it's a neighbour to add */
        else if (entriesRead == 2) {
            /* check if the city that is pointed by the neighbour is in the map */
            c2 = searchCity(m, value1);

            /* if the city is not found create a new city before creating the neighbour */
            if (!c2) {
                /* create a new city, otherwise delete the map and return 0 if fails */
                c2 = newCity(value1);
                if (!c2) {
                    delMap(m);
                    return 0;
                }

                /* add the new city to the map, otherwise delete the map and return 0 if fails */
                st = addCity(m, c2);
                if (st) {
                    delMap(m);
                    fprintf(stderr, "%s\n", message(st));
                    return 0;
                }
            }

            /* create a new neighbour that point to c2, otherwise delete the map and return 0 if fails */
            n = newNeighbour(c2, value2);
            if (!n) {
                delMap(m);
                return 0;
            }

            /* add the new neighbour to the city, otherwise delete the map and return 0 if fails */
            st = addNeighbour(c1, n);
            if (st) {
                delMap(m);
                fprintf(stderr, "%s\n", message(st));
                return 0;
            }
        }
    }
    free(line);
    return m;
}
