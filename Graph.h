#ifndef __Graph_H
#define __Graph_H

#include "List.h"

typedef struct Vertex {
  char *key;
  int dist;
  int h;
  int lng;
  int lat;
  struct Vertex *back;
  List *neighbours;
} Vertex;

typedef struct {
  Vertex *vertex;
  int dist;
} Neighbour;

typedef List Graph;

Vertex *searchVertex(List *g, char *key);

Graph *newGraph();

Neighbour *newNeighbour();

Vertex *newVertex(char *key);

void addNeighbour(Vertex *v, Neighbour *n);

void addVertex(Graph *g, Vertex *v);

void delGraph(Graph *g);

void aStar(Graph *g, Vertex *start, Vertex *goal);

#endif