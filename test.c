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

typedef struct {
    char * key;
    int lng;
    int lat;
    int h;
    int dist;
    List * neighbours;
} City;

typedef struct {
    char * key;
    int dist;
} Neighbour;

typedef struct {
    char * key;
} Key;



void aStar(List * g);

static int compCity(void * c1, void * c2) {
    return strcmp(((Key *)c1)->key, ((Key *)c2)->key);
}

static void prCity(void * c) { 
    printf("{ key: %s, lng: %d, lat: %d }", ((City *)c)->key, ((City *)c)->lng, ((City *)c)->lat);
}

static int compNeighbour(void * n1, void * n2) {
    return strcmp(((Key *)n1)->key, ((Key *)n2)->key);
}

static void prNeighbour(void * n) { 
    printf("{ key: %s, dist: %d }", ((Neighbour *)n)->key, ((Neighbour *)n)->dist);
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
    List * neighbours;
    List * cities;
    
    cities = newList(&compCity, &prCity);
    City * city; 
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
            if (lengthList(data) == 3) {
                char * key;
                city = (City *)malloc(sizeof(City));

                nthInList(data, 1, &key);
                city -> key = strdup(key);

                char * strLat;
                nthInList(data, 2, &strLat);
                city -> lat = atoi(strLat);


                char *strLng;
                nthInList(data, 3, &strLng);
                city -> lng = atoi(strLng);

                city->neighbours = newList(&compNeighbour, &prNeighbour);

                addList(cities, city); 
            } else if (lengthList(data) == 2){
                char * key;
                char * strCost;
                Neighbour * neighbour;

                neighbour = malloc(sizeof(Neighbour));

                nthInList(data, 1, &key);
                neighbour -> key = key;
            
                nthInList(data, 2, &strCost);
                neighbour -> dist = atoi(strCost);

                addList(city -> neighbours, neighbour);
            }
          
        }
          displayList(cities);
        //   for (int i = 1; i <= lengthList(cities)) {
        //     free()
        //   }


    aStar(cities);    
    return 0;
}

void aStar(List * g){


    // int inf;
    // List * open;
    // List * closed;
    // Key * start;
    // Key * goal;

    // start -> key = "Paris";
    // goal -> key = "Lyon";

    // // if(isInList(g, start)) {
    // //     printf("yes");
    // // }


    // inf = 10000;

    // open = newList(0, 0);
    // closed = newList(0, 0);


    // free(open);
    // free(closed);
}