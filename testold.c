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

typedef struct {
  char *key;
} Key;

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
  int dist;
} Neighbour;

void aStar(List *g);

static int compVertex(void *v1, void *v2) {
  return strcmp(((Vertex *)v1)->key, ((Vertex *)v2)->key);
}

static void prVertex(void *v) {
  printf("{ key: %s, lng: %d, lat: %d, h: %d, dist:%d  }", ((Vertex *)v)->key,
         ((Vertex *)v)->lng, ((Vertex *)v)->lat, ((Vertex *)v)->h,
         ((Vertex *)v)->dist);
}

static int compNeighbour(void *n1, void *n2) {
  return strcmp(((Key *)n1)->key, ((Key *)n2)->key);
}

static void prNeighbour(void *n) {
  printf("{ key: %s, dist: %d }", ((Neighbour *)n)->key,
         ((Neighbour *)n)->dist);
}

static int compVertexByf(void *v1, void *v2) {
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
      vertex = (Vertex *)malloc(sizeof(Vertex));

      nthInList(data, 1, &key);
      vertex->key = strdup(key);

      char *strLat;
      nthInList(data, 2, &strLat);
      vertex->lat = atoi(strLat);

      char *strLng;
      nthInList(data, 3, &strLng);
      vertex->lng = atoi(strLng);

      vertex->neighbours = newList(&compNeighbour, &prNeighbour);

      addList(vertices, vertex);
    } else if (lengthList(data) == 2) {
      char *key;
      char *strCost;
      Neighbour *neighbour;

      neighbour = malloc(sizeof(Neighbour));

      nthInList(data, 1, &key);
      neighbour->key = strdup(key);

      nthInList(data, 2, &strCost);
      neighbour->dist = atoi(strCost);

      addList(vertex->neighbours, neighbour);
    }
  }
  //   displayList(vertices);

  aStar(vertices);
  return 0;
}

void displayPath(Vertex *v) {
  if (v->back)
    displayPath(v->back);
  printf("%s : (%d km)\n", v->key, v->dist);
}

void aStar(List *g) {

  int inf;
  List *open;
  List *closed;
  Vertex abc;
  Vertex *start;
  Vertex *goal;
  Vertex cde;

  abc.key = "Rennes";
  cde.key = "Lyon";

  inf = 10000;
  Vertex *v;
  open = newList(&compVertexByf, &prVertex);
  closed = newList(&compVertex, &prVertex);

  Node *node = isInList(g, &abc);
  start = (Vertex *)node->next->val;

  node = isInList(g, &cde);
  goal = (Vertex *)node->next->val;

  for (int i = 1; i <= g->nelts; i++) {
    nthInList(g, i, &v);
    if (strcmp(v->key, start->key)) {
      v->dist = inf - i;
      v->h = abs(goal->lng - v->lng) / 4 + abs(goal->lat - v->lat) / 4;
    }
  }

  start->dist = 0;
  start->h = 0;
  addList(open, start);

  while (lengthList(open)) {
    Vertex *current;
    remFromListAt(open, 1, &current);
    addList(closed, current);

    if (!strcmp(current->key, goal->key)) {
      displayPath(current);
      break;
    }

    for (int i = 1; i <= current->neighbours->nelts; i++) {
      Neighbour *n;
      nthInList(current->neighbours, i, &n);
      Node *node = isInList(g, n);
      Vertex *tmp;
      if (node == (Node *)1) {
        nthInList(g, 1, &tmp);
      } else if (node != 0) {
        tmp = (Vertex *)node->next->val;
      }

      if (tmp->dist > current->dist + n->dist) {
        tmp->dist = current->dist + n->dist;
        tmp->back = current;

        open->comp = &compVertex;
        if (isInList(open, tmp) != 0) {
          remFromList(open, tmp);
        }
        open->comp = &compVertexByf;

        if (isInList(closed, tmp) != 0)
          remFromList(closed, tmp);

        addList(open, tmp);
        displayList(open);
        puts("");
      }
      // displayList(open);
      // puts("");
      // displayList(closed);
    }
  }
}

void test(List *g, Vertex *start, Vertex *goal) {
  int inf;
  Neighbour *tmpNeighbour;
  Vertex *start, *goal, *tmpVertex;
  List *open, *closed;
  Node *tmpNode;

  inf = 10000;
  for (int i = 1; i <= lengthList(g); i++) {
    nthInList(g, i, &tmpVertex);
    if (strcmp(tmpVertex->key, start->key)) {
      tmpVertex->dist = inf - i;
      tmpVertex->h = abs(goal->lng - tmpVertex->lng) / 4 +
                     abs(goal->lat - tmpVertex->lat) / 4;
    }
  }

  open = newList(&compVertexByf, &prVertex);
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
      tmpNode = isInList(g, tmpNeighbour);

      if (tmpNode == (Node *)1) {
        nthInList(g, 1, &tmpVertex);
      } else if (tmpNode != 0) {
        tmpVertex = (Vertex *)tmpNode->next->val;
      }

      if (tmpVertex->dist > current->dist + tmpNeighbour->dist) {
        tmpVertex->dist = current->dist + tmpNeighbour->dist;
        tmpVertex->back = current;

        open->comp = &compVertex;
        if (isInList(open, tmpVertex) != 0) {
          remFromList(open, tmpVertex);
        }
        open->comp = &compVertexByf;

        if (isInList(closed, tmpVertex) != 0)
          remFromList(closed, tmpVertex);

        addList(open, tmpVertex);
        displayList(open);
      }
    }
  }
}