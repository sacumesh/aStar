#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compString(void *s1, void *s2) {
  return strcmp((char *)s1, (char *)s2);
}

/*************************************************************
 * Function to display an element of the list
 * @param s the string to display
 *************************************************************
 */
static void prString(void *s) { printf("%s", (char *)s); }
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
  char *key;
  Vertex *vertex;
  int dist;
} Neighbour;

void aStar(List *g, Vertex *start, Vertex *goal);

static int compVertex(void *v1, void *v2) {
  return strcmp(((Vertex *)v1)->key, ((Vertex *)v2)->key);
}

static void prVertex(void *v) {
  printf("{ key: %s, lng: %d, lat: %d, h: %d, dist:%d  }", ((Vertex *)v)->key,
         ((Vertex *)v)->lng, ((Vertex *)v)->lat, ((Vertex *)v)->h,
         ((Vertex *)v)->dist);
}

static int compNeighbour(void *n1, void *n2) {
  return strcmp(((Neighbour *)n1)->key, ((Neighbour *)n2)->key);
}

static void prNeighbour(void *n) {
  printf("{ key: %s, dist: %d }", ((Neighbour *)n)->key,
         ((Neighbour *)n)->dist);
}

static int compVertexByCombinedCost(void *v1, void *v2) {
  return ((Vertex *)v1)->dist + ((Vertex *)v1)->h - ((Vertex *)v2)->dist -
         ((Vertex *)v2)->h;
}

List *split(char *str, char *seperators) {
  List *list = newList(&compString, &prString);
  char *token = strtok(str, seperators);
  while (token != 0) {
    addListAt(list, list->nelts + 1, token);
    token = strtok(0, seperators);
  }
  return list;
}

int main() {
  FILE *f;
  size_t sz, len;
  char *line;
  List *data;
  List *neighbours;
  List *vertices;

  vertices = newList(&compVertex, &prVertex);
  Vertex *vertex;
  f = fopen("FRANCE.MAP", "r");
  while ((len = getline(&line, &sz, f)) != -1) {
    if (len == 1)
      continue;

    for (int i = 0; i < len; i++) {
      if (line[i] == '\n') {
        line[i] = '\0';
        break;
      }
    }
    data = split(line, "\t ");
    if (lengthList(data) == 3) {
      char *key;
      Vertex v;
      nthInList(data, 1, &key);
      v.key = key;
      Node *node = isInList(vertices, &v);

      if (node == (Node *)1) {
        nthInList(vertices, 1, &vertex);
      } else if (node != 0) {
        vertex = (Vertex *)node->next->val;
      } else {
        vertex = (Vertex *)malloc(sizeof(Vertex));
        vertex->key = strdup(key);
        addList(vertices, vertex);
      }

      //   printf("%s",vertex->key);

      char *strLat;
      nthInList(data, 2, &strLat);
      vertex->lat = atoi(strLat);

      char *strLng;
      nthInList(data, 3, &strLng);
      vertex->lng = atoi(strLng);

      vertex->neighbours = newList(&compNeighbour, &prNeighbour);

    } else if (lengthList(data) == 2) {
      char *key;
      char *strCost;
      Neighbour *neighbour;
      Vertex v;

      neighbour = malloc(sizeof(Neighbour));

      nthInList(data, 1, &key);
      neighbour->key = strdup(key);

      nthInList(data, 2, &strCost);
      neighbour->dist = atoi(strCost);
      v.key = key;

      Node *node = isInList(vertices, &v);
      Vertex *tmp;
      if (node == (Node *)1) {
        // printf("%s", v.key);
        nthInList(vertices, 1, &tmp);
      } else if (node != 0) {
        tmp = (Vertex *)node->next->val;
      } else {
        tmp = (Vertex *)malloc(sizeof(Vertex));
        tmp->key = strdup(key);
        addList(vertices, tmp);
        // printf("%s\n", tmp -> key);
        // displayList(vertices);
        // puts("");
      }
      neighbour->vertex = tmp;

      addList(vertex->neighbours, neighbour);
    }
  }
  //   displayList(vertices);

  Vertex abc;
  Vertex *start;
  Vertex *goal;
  Vertex cde;

  abc.key = "Rennes";
  cde.key = "Lyon";

  Vertex *v;

  Node *node = isInList(vertices, &abc);
  start = (Vertex *)node->next->val;

  node = isInList(vertices, &cde);
  goal = (Vertex *)node->next->val;

  aStar(vertices, start, goal);
  return 0;
}

void displayPath(Vertex *v) {
  if (v->back)
    displayPath(v->back);
  printf("%s : (%d km)\n", v->key, v->dist);
}

void aStar(List *g, Vertex *start, Vertex *goal) {
  int inf;
  Neighbour *tmpNeighbour;
  Vertex *tmpVertex;
  List *open, *closed;
  Node *tmpNode;

  inf = 10000;
  for (int i = 1; i <= lengthList(g); i++) {
    nthInList(g, i, &tmpVertex);
    if (compVertex(tmpVertex, start)) {
      tmpVertex->dist = inf - i;
      tmpVertex->h = abs(goal->lng - tmpVertex->lng) / 4 +
                     abs(goal->lat - tmpVertex->lat) / 4;
    }
  }

  open = newList(&compVertexByCombinedCost, &prVertex);
  closed = newList(&compVertex, &prVertex);

  start->dist = 0;
  start->h = 0;
  addList(open, start);

  while (lengthList(open)) {
    Vertex *current;
    remFromListAt(open, 1, &current);
    addList(closed, current);

    if (!compVertex(current, goal)) {
      displayPath(current);
      break;
    }

    for (int i = 1; i <= lengthList(current->neighbours); i++) {
      nthInList(current->neighbours, i, &tmpNeighbour);
      Vertex *v = tmpNeighbour->vertex;

      if (v->dist > current->dist + tmpNeighbour->dist) {
        v->dist = current->dist + tmpNeighbour->dist;
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
}