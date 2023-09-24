#ifndef __Map_H
#define __Map_H

#include "List.h"

/** Map is a typedef for the List data structure */
typedef List Map;

/** Neighbours is a typedef for the List data structure */
typedef List Neighbours;

/**
 * City represents a city entity in a map.
 * The key represents a unique identifier for the city
 * distStart represents the distance from the starting point or source city.
 * distGoal represents the estimated distance from the goal or destination city
 * lng represents the longitude coordinate of the city's location.
 * lat represents the latitude coordinate of the city's location.
 * back is a pointer to the previous city in a path or route
 * neighbours is a pointer to the list of neighbouring cities
 */
typedef struct City {
    char *key;
    int distStart;
    int distGoal;
    int lng;
    int lat;
    struct City *back;
    Neighbours *neighbours;
} City;

/**
 * Neighbour represents a neighbouring city entity associated with a city in a map.
 * city is a pointer to the city the neighbour is associated with.
 * dist represents the distance from the city which this neighbour is a neighbour
 */
typedef struct {
    City *city;
    int dist;
} Neighbour;


/** Empty Map creation by using then generic List.
 * @return a new (empty) map if memory allocation is successful
 * @return 0 otherwise
 */
Map *newMap();

/**
 * Creates a new City with the specified key.
 * @param key the key associated with the city
 * @return a pointer to the newly created City object if memory allocation is successful
 * @return 0 otherwise
 */
City *newCity(char *key);

/**
 * Creates a new Neighbour with the specified City and distance.
 * @param c a pointer to the City object representing the neighbor
 * @param dist the distance from the a city to the neighbor
 * @return a pointer to the newly created Neighbour object if memory allocation is successful
 * @return 0 otherwise
 */
Neighbour *newNeighbour(City *c, int dist);

/**
 * Searches for a City with the specified key in the given Map.
 * @param m a pointer to the Map in which to search for the City
 * @param key the key associated with the City to be searched
 * @return a pointer to the found City if it exists in the Map
 * @return 0 if the City is not found or if memory allocation fails
 */
City *searchCity(Map *m, char *key);

/**
 * Add a neighbour to a city.
 * @param c the city to add the neighbour to
 * @param n the neighbour to add
 * @return OK if the neighbour was added successfully
 * @return ERRADDNEIGHBOUR if an error occurred while adding the neighbour
 */
status addNeighbour(City *c, Neighbour *n);

/**
 * Add a city to the map.
 * @param m the map to add the city to
 * @param c the city to add
 * @return OK if the city was added successfully
 * @return ERRADDCITY if an error occurred while adding the city
 */
status addCity(Map *m, City *c);

/** destroy the memory by deallocating used memory.
 * @param m the map to destroy
 */
void delMap(Map *m);

/**
 * Performs the A* search algorithm to find the optimal path between two cities on a map.
 * @param m The map containing the cities and their connections.
 * @param start The starting city for the path.
 * @param goal The goal city to reach.
 * @return OK if a shortest path from start to goal was found
 * @return ERRMAPCONNECTIVITY if the map lacks connectivity
 * @return ERRASTAR if an error occurred while running the A* search algorithm.
 */
status aStar(Map *m, City *start, City *goal);

#endif
