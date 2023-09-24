#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "Map.h"

/**
 * Function to compare two elements (City) : based on strcasecmp
 * @param c1 the first City to compare
 * @param c2 the second Neighbour to compare
 * @return <0 if c1 is less than c2
 * @return 0 if c1 equals c2
 * @return >0 otherwise
 */
static int cmpCity(void *c1, void *c2) {
    return strcasecmp(((City *) c1)->key, ((City *) c2)->key);
}

/**
 * Function to display an element (City) of the list
 * @param c the neighbour to display
 */
static void prCity(void *c) {
    printf("{ key: %s, lng: %d, lat: %d, distGoal: %d, distStart:%d  }", ((City *) c)->key,
           ((City *) c)->lng, ((City *) c)->lat, ((City *) c)->distGoal,
           ((City *) c)->distStart);
}

/**
 * Another Function to compare two elements (City) : based on the sum of distStart and distGoal
 * @param c1  first City to compare
 * @param c2 the second Neighbour to compare
 * @return <0 if c1 is less than c2
 * @return 0 if c1 equals c2
 * @return >0 otherwise
 */
static int compCityByCombinedDist(void *c1, void *c2) {
    return ((City *) c1)->distStart + ((City *) c1)->distGoal - ((City *) c2)->distStart -
           ((City *) c2)->distGoal;
}

/**
 * Function to compare two elements (Neighbour) : based on strcasecmp
 * @param n1 the first Neighbour to compare
 * @param n2 the second Neighbour to compare
 * @return <0 if n1 is less than n2
 * @return 0 if n1 equals n2
 * @return >0 otherwise
 */
static int cmpNeighbour(void *n1, void *n2) {
    return strcasecmp(((Neighbour *) n1)->city->key, ((Neighbour *) n2)->city->key);
}

/**
 * Function to display an element (Neighbour) of the list
 * @param n the neighbour to display
 */
static void prNeighbour(void *n) {
    printf("{ key: %s, distStart: %d }", ((Neighbour *) n)->city->key,
           ((Neighbour *) n)->dist);
}

/**
 * Recursive function to display the path to the given city
 * @param c the destination city to which the path is displayed
 */
static void displayPath(City *c) {
    if (c->back)
        displayPath(c->back);
    printf("%s : (%d km)\n", c->key, c->distStart);
}

/* create a new, empty map */
Map *newMap() {
    Map *res;
    res = newList(&cmpCity, &prCity);
    if (!res)
        return 0;
    return res;
}

/* create a new city with the specified key */
City *newCity(char *key) {
    City *res;
    List *ns;
    res = (City *) malloc(sizeof(City));
    if (!res)
        return 0;

    res->back = 0;

    /* allocate memory for the key and copy the specified string */
    res->key = strdup(key);

    if (!res->key) {
        free(res);
        return 0;
    }


    /* Create a new List for storing the Neighbours of the City */
    ns = newList(&cmpNeighbour, &prNeighbour);
    if (!ns) {
        free(res);
        return 0;
    }

    res->neighbours = ns;

    return res;
}

/* create a new neighbour pointing to the specified City and the given distance */
Neighbour *newNeighbour(City *v, int dist) {
    Neighbour *res;
    res = (Neighbour *) malloc(sizeof(Neighbour));
    if (!res)
        return 0;
    res->city = v;
    res->dist = dist;
    return res;
}

/* add a city to the map. */
status addCity(Map *m, City *c) {
    status st;

    /* Add the city to the map using the addList function */
    st = addList(m, c);

    if (st) {
        /* Print an error message to the standard error stream */
        fprintf(stderr, "%s\n", message(st));
        return ERRADDCITY;
    }

    return OK;
}

/* add a neighbour to the city. */
status addNeighbour(City *c, Neighbour *n) {
    status st;
    /* Add the neighbour to the city's list of neighbours using the addList function */
    st = addList(c->neighbours, n);

    if (st) {
        /* Print an error message to the standard error stream */
        fprintf(stderr, "%s\n", message(st));
        return ERRADDNEIGHBOUR;
    }

    return OK;
}

/* destroy the map by deallocating used memory */
void delMap(Map *m) {
    int i;
    City *c;
    for (i = 1; i <= m->nelts; i++) {
        /* get the ith city in the map */
        nthInList(m, i, &c);

        /* delete the neighbours of the ith city */
        delList(c->neighbours);

        /* deallocate memory of the city key */
        free(c->key);
    }

    /* delete the list that holds the cities */
    delList(m);
}

/* searches for the city with the specified key in the map */
City *searchCity(Map *m, char *key) {
    City *res;
    City c;
    Node *node;

    c.key = key;
    // Find the city which correspond to the given key
    node = isInList(m, &c);

    // if not found return 0;
    if (!node)
        return 0;

    // if found set the res to the found city
    if (node == (Node *) 1) {
        nthInList(m, 1, &res);
    } else {
        res = (City *) node->next->val;
    }

    return res;
}

status aStar(Map *m, City *start, City *goal) {
    int inf;
    Neighbour *n;
    City *c, *current;
    List *open, *closed;
    status st;

    /* infinity to initialize the graph cost */
    inf = 10000;

    /*  initialize distances */
    for (int i = 1; i <= lengthList(m); i++) {
        /* get the 'i'-th city from the list 'm' */
        nthInList(m, i, &c);

        /* If the city 'c' is not the start city, update its distance from the start and goal */
        if (cmpCity(c, start)) {

            /* set the distance from the start as infinity*/
            c->distStart = inf - i;
        }
        /* calculate the estimated distance (heuristic) to the goal using longitude and latitude differences */
        c->distGoal = abs(goal->lng - c->lng) / 4 + abs(goal->lat - c->lat) / 4;
    }

    /* initialize distances for start */
    start->distStart = 0;

    /* initialize open, otherwise return ERRASTAR if fails */
    open = newList(&compCityByCombinedDist, &prCity);
    if (!open) return ERRASTAR;

    /* add star city to open, otherwise return ERRASTAR if fails */
    st = addList(open, start);
    if (st) {
        delList(open);
        fprintf(stderr, "%s\n", message(st));
        return ERRASTAR;
    }

    /* initialize closed, otherwise return ERRASTAR if fails */
    closed = newList(&cmpCity, &prCity);
    if (!closed) {
        delList(open);
        return ERRASTAR;
    }

    while (lengthList(open)) {
        /* remove the city with the minimal f value (distGoal + distStart)  and set it to current */
        remFromListAt(open, 1, &current);

        /* add the current to closed, otherwise return ERRASTAR if fails */
        st = addList(closed, current);
        if (st) {
            delList(open);
            delList(closed);
            fprintf(stderr, "%s\n", message(st));
            return ERRASTAR;
        };

        /* if the current city is the goal display path (with cost) from start city to goal city and return OK */
        if (!cmpCity(current, goal)) {
            displayPath(current);
            delList(open);
            delList(closed);
            return OK;
        }

        for (int i = 1; i <= lengthList(current->neighbours); i++) {
            /* get the 'i'-th neighbour from the current city's neighbours list */
            nthInList(current->neighbours, i, &n);

            /* assign the city of the obtained neighbour to 'c' */
            c = n->city;

            /*  continue to the next successor if the city 'c' is already in the open or closed list
             *  and if the distance from the start to city 'c' is less the sum of the distance from the
             *  start to the current city and the distance between the current city and city 'c' */
            if ((isInList(open, c) || isInList(closed, c)) && c->distStart < current->distStart + n->dist)
                continue;

            /* update the distance from the start to city 'c */
            c->distStart = current->distStart + n->dist;

            /* set the predecessor of city 'c' to 'current */
            c->back = current;

            /* if the city 'c' is already in the 'closed' list, remove it */
            if (isInList(closed, c))
                remFromList(closed, c);

            /* if the city 'c' is already in the 'remove' list, remove it */
            open->comp = &cmpCity;
            if (isInList(open, c))
                remFromList(open, c);

            /* Add city 'c' to the 'open' list (rearrange to keep it sorted based on f value (distGoal + distStart) )
             * otherwise return ERRASTAR if fails */
            open->comp = &compCityByCombinedDist;
            st = addList(open, c);
            if (st) {
                delList(open);
                delList(closed);
                fprintf(stderr, "%s\n", message(st));
                return ERRASTAR;
            }
        }
    }

    /* delete the 'open' list and free the allocated memory */
    delList(open);

    /* delete the 'closed' list and free the allocated memory */
    delList(closed);

    /* return error code indicating lack of connectivity in the map */
    return ERRMAPCONNECTIVITY;
}
