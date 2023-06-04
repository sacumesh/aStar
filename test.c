#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
Vertex *searchVertex(List *g, char *key);
void processLine(char* line);

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

List *t(FILE *file) {
  char* line;
  size_t len;
  ssize_t read;
  char value1[100];
  int value2;
  int value3;
  List *g;
  Vertex *v1, *v2;
  Neighbour *n;
  
  g = newList(&compVertex, &prVertex);
  while ((read = getline(&line, &len, file)) != -1) {
    int entriesRead = sscanf(line, "%s %d %d", value1, &value2, &value3);
    if (entriesRead == 3) {
      v1 = searchVertex(g, value1);
      if (!v1) {
        v1 = (Vertex *)malloc(sizeof(Vertex));
        v1->key = strdup(value1);
        addList(g, v1);
      }
      v1->lat = value2;
      v1->lng = value3;
      v1->neighbours = newList(&compNeighbour, &prNeighbour);
    } else if (entriesRead == 2) {
      v2 = searchVertex(g, value1);

      if (!v2) {
        v2 = (Vertex *)malloc(sizeof(Vertex));
        v2->key = strdup(value1);
        addList(g, v2);
      }

      n = (Neighbour *)malloc(sizeof(Neighbour));
      n->dist = value2;
      n->vertex = v2;
      addList(v1->neighbours, n);
    }
  }
  return g;
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

void processLine(char* line) {
    // Process the line as needed
    printf("Processing line: %s", line);
}

void readLinesFromFile(FILE* file) {
    char* line;
    size_t len;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        processLine(line);
    }

    // No need to free(line) here
}

int main(int nWords, char *words[]) {
  Vertex *start;
  Vertex *goal;
  List *g;
  FILE *f;
  char startStr[20];
  char goalStr[20];

  FILE* file = fopen("FRANCE.MAP", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    t(file);

    // fclose(file);

  // f = fopen("FRANCE.MAP", "r");
  // g = t(&f);
  // fclose(f);

  // if (nWords>=3){
  //   strcpy(startStr, words[1]);
  //   strcpy(goalStr, words[2]);
  // } else {
  //   printf("Enter start? ");
  //   scanf("%s", startStr);
  //   printf("Enter goal? ");
  //   scanf("%s", goalStr);
  // }

  // start = searchVertex(g, startStr);
  // if (!start)
  //   return 1;

  // goal = searchVertex(g, goalStr);
  // if (!goal)
  //   return 1;

  // aStar(g, start, goal);
}