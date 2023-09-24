#include <stdio.h>
#include <string.h>
#include "Map.h"

/* Maximum length for character arrays of read from file line */
#define MAX_LINE_LENGTH 100

/* Maximum length for character arrays of city name */
#define MAX_CITY_LENGTH 50

/* Default file to load the map */
#define DEFAULT_MAP_FILE "./FRANCE.MAP"

/**
 * Creates a map object by reading data from a file
 * The file should contain information about cities and their connections
 * @param file The file pointer to read the map data from
 * @return A pointer to the created map object if successful
 * @return 0 otherwise
 */
Map *createMapFromFile(FILE *file);

int main(int nWords, char *words[]) {
    City *start;
    City *goal;
    Map *m;
    status st;
    char startStr[MAX_CITY_LENGTH];
    char goalStr[MAX_CITY_LENGTH];
    FILE *file;

    /* open the file for reading */
    file = fopen(DEFAULT_MAP_FILE, "r");

    /* return the error code 2 if the file cannot be opened */
    if (!file) {
        fprintf(stderr, "%s\n", "File open failed");
        return 2;
    }

    /* create a map from the file */
    m = createMapFromFile(file);

    /* close the file, return the error code 2 if the file cannot be closed */
    if (fclose(file)) {
        fprintf(stderr, "%s\n", "File close failed");
        return 2;
    };

    /* return the error code 4 if the map creation fails */
    if (!m) {
        fprintf(stderr, "%s\n", "Map creation failed");
        return 4;
    }

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

    /* find the start city in the map, otherwise return the error code 1 if the start city is not found */
    start = searchCity(m, startStr);
    if (!start) {
        fprintf(stderr, "City '%s' not found\n", startStr);
        return 1;
    }

    /* find the goal city in the map, otherwise return the error code 1 if the goal city is not found  */
    goal = searchCity(m, goalStr);
    if (!goal) {
        fprintf(stderr, "City '%s' not found \n", goalStr);
        return 1;
    }

    /*  Call the A* search algorithm to find a shortest path
     *  If the status is ERRMAPCONNECTIVITY, return 3 to indicate a lack of map connectivity
     *  Otherwise, return 4 for other error conditions */
    st = aStar(m, start, goal);
    if (st) {
        /*  print the error message associated with the status */
        fprintf(stderr, "%s\n", message(st));
        if (st == ERRMAPCONNECTIVITY) return 3;

        return 4;
    }

    /* Deletes the map object from memory */
    delMap(m);
    return 0;
}

/* creates a map from a given file, otherwise return 0 if fails */
Map *createMapFromFile(FILE *file) {
    char line[MAX_LINE_LENGTH];
    char value1[MAX_CITY_LENGTH];
    int value2;
    int value3;
    Map *m;
    status st;
    City *c1, *c2;
    Neighbour *n;

    /* create a new map, otherwise return 0 if fails */
    m = newMap();
    if (!m) return 0;

    c1 = c2 = 0;
    while (fgets(line, sizeof(line), file)) {
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
    
    return m;
}
