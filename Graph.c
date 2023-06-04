#include "Graph.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compVertex(void *v1, void *v2) {
  return strcmp(((Vertex *)v1)->key, ((Vertex *)v2)->key);
}

static void prVertex(void *v) {
  printf("{ key: %s, lng: %d, lat: %d, h: %d, dist:%d  }", ((Vertex *)v)->key,
         ((Vertex *)v)->lng, ((Vertex *)v)->lat, ((Vertex *)v)->h,
         ((Vertex *)v)->dist);
}

static int compNeighbour(void *n1, void *n2) {
  return strcmp(((Neighbour *)n1)->vertex->key, ((Neighbour *)n2)->vertex->key);
}

static void prNeighbour(void *n) {
  printf("{ key: %s, dist: %d }", ((Neighbour *)n)->vertex->key,
         ((Neighbour *)n)->dist);
}

static int compVertexByCombinedCost(void *v1, void *v2) {
  return ((Vertex *)v1)->dist + ((Vertex *)v1)->h - ((Vertex *)v2)->dist -
         ((Vertex *)v2)->h;
}

static void displayPath(Vertex *v) {
  if (v->back)
    displayPath(v->back);
  printf("%s : (%d km)\n", v->key, v->dist);
}

Graph *newGraph() {
  Graph *res;
  res = newList(&compVertex, &prVertex);
  if (!res)
    return 0;
  return res;
}

Vertex *newVertex(char *key) {
  Vertex *res;
  Neighbours *ns;
  res = (Vertex *)malloc(sizeof(Vertex));
  if (!res)
    return 0;
  ns = newList(&compNeighbour, &prNeighbour);
  res->neighbours = ns;
  res->key = key;
  return res;
}

Neighbour *newNeighbour() {
  Neighbour *res;
  res = (Neighbour *)malloc(sizeof(Neighbour));
  if (!res)
    return 0;
  return res;
}

void addVertex(Graph *g, Vertex *v) {
  addList(g, v);
}

void addNeighbour(Vertex *v, Neighbour *n) {
  addList(v->neighbours, n);
}

void delGraph(Graph *g) {
  int i;
  Vertex *v;
  for (i = 1; i < g->nelts; i++) {
    nthInList(g, i, &v);
    delList(v->neighbours);
    free(v->key);
  }

  delList(g);
}

Vertex *searchVertex(List *g, char *key) {
  Vertex *res;
  Vertex v;
  Node *node;

  v.key = key;
  node = isInList(g, &v);
  if (!node)
    return 0;

  if (node == (Node *)1) {
    nthInList(g, 1, &res);
  } else {
    res = (Vertex *)node->next->val;
  }
  return res;
}

void aStar(Graph *g, Vertex *start, Vertex *goal) {
  int inf;
  Neighbour *n;
  Vertex *v, *current;
  List *open, *closed;

  inf = 10000;
  for (int i = 1; i <= lengthList(g); i++) {
    nthInList(g, i, &v);
    if (compVertex(v, start)) {
      v->dist = inf - i;
      v->h = abs(goal->lng - v->lng) / 4 + abs(goal->lat - v->lat) / 4;
    }
  }

  open = newList(&compVertexByCombinedCost, &prVertex);
  closed = newList(&compVertex, &prVertex);

  start->dist = 0;
  start->h = 0;
  addList(open, start);

  while (lengthList(open)) {
    remFromListAt(open, 1, &current);
    addList(closed, current);

    if (!compVertex(current, goal)) {
      displayPath(current);
      break;
    }

    for (int i = 1; i <= lengthList(current->neighbours); i++) {
      nthInList(current->neighbours, i, &n);
      v = n->vertex;

      if (v->dist > current->dist + n->dist) {
        v->dist = current->dist + n->dist;
        v->back = current;

        open->comp = &compVertex;
        if (isInList(open, v))
          remFromList(open, v);
        open->comp = &compVertexByCombinedCost;

        if (isInList(closed, v))
          remFromList(closed, v);

        addList(open, v);
      }
    }
  }

  delList(open);
  delList(closed);
  printf("test");
}
