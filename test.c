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
static void prString(void *s) {
  printf("%s", (char *)s);
}
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

List *split(char *str, char *seperators) {
  List *list = newList(&compString, &prString);
  char *token = strtok(str, seperators);
  while (token != 0) {
    addListAt(list, list->nelts + 1, token);
    token = strtok(0, seperators);
  }
  return list;
}

List *test() {
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

      nthInList(data, 2, &strCost);
      neighbour->dist = atoi(strCost);
      v.key = key;

      Node *node = isInList(vertices, &v);
      Vertex *tmp;
      if (node == (Node *)1) {
        nthInList(vertices, 1, &tmp);
      } else if (node != 0) {
        tmp = (Vertex *)node->next->val;
      } else {
        tmp = (Vertex *)malloc(sizeof(Vertex));
        tmp->key = strdup(key);
        addList(vertices, tmp);
      }
      neighbour->vertex = tmp;

      addList(vertex->neighbours, neighbour);
    }
  }
  return vertices;
}

List * t() {
  FILE *f;
  size_t sz, len;
  char *line;
  char num1[100];
  int num2;
  int num3;
  List *g;
  Vertex * v;
  Neighbour * n;
  Node *node;


  f = fopen("FRANCE.MAP", "r");
  g = newList(&compVertex, &prVertex);
  while ((len = getline(&line, &sz, f)) != -1) {
    int entriesRead = sscanf(line, "%s %d %d", num1, &num2, &num3);
      if (entriesRead == 3) {
        Vertex vKey;
        vKey.key = num1;
        Node *node = isInList(g, &vKey);

      if (node == (Node *)1) {
        nthInList(g, 1, &v);
      } else if (node != 0) {
        v = (Vertex *)node->next->val;
      } else {
        v = (Vertex *)malloc(sizeof(Vertex));
        v -> key = strdup(num1);
        addList(g, v);
      }

        v -> lat = num2;
        v -> lng = num3;
        v -> neighbours = newList(&compNeighbour, &prNeighbour);
      } else if(entriesRead == 2) {
        Vertex * tmp;
        Vertex tmpV;
        tmpV.key = num1;

        Node *node = isInList(g, &tmpV);

        if (node == (Node *)1) {
        nthInList(g, 1, &tmp);
      } else if (node != 0) {
        tmp = (Vertex *)node->next->val;
      } else {
        tmp = (Vertex *)malloc(sizeof(Vertex));
        tmp->key = strdup(num1);
        addList(g, tmp);
      }

        n = (Neighbour *)malloc(sizeof(Neighbour));
        n -> dist = num2;
        n -> vertex = tmp; 
        addList(v -> neighbours, n);
      }
  }
  fclose(f);
  return g;
}

int main() {
  List *g = t();

  Vertex abc;
  Vertex *start;
  Vertex *goal;
  Vertex cde;

  abc.key = "Rennes";
  cde.key = "Lyon";

  Vertex *v;

  Node *node = isInList(g, &abc);
  start = (Vertex *)node->next->val;

  node = isInList(g, &cde);
  goal = (Vertex *)node->next->val;

  aStar(g, start, goal);
  return 0;
}

void displayPath(Vertex *v) {
  if (v->back)
    displayPath(v->back);
  printf("%s : (%d km)\n", v->key, v->dist);
}

void aStar(List *g, Vertex *start, Vertex *goal) {
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
}