#include <stdio.h>
#include <string.h>
#include "Graph.h"



List *createGraphFromFile(FILE *file);

int main(int nWords, char *words[]) {
  Vertex *start;
  Vertex *goal;
  List *g;
  FILE *f;
  char startStr[20];
  char goalStr[20];

  FILE* file = fopen("FRANCE.MAP", "r");
  if (!file) return 2;

  g = createGraphFromFile(file);

  if (nWords>=3) {
    strcpy(startStr, words[1]);
    strcpy(goalStr, words[2]);
  } else {
    printf("Enter start? ");
    scanf("%s", startStr);
    printf("Enter goal? ");
    scanf("%s", goalStr);
  }

  start = searchVertex(g, startStr);
  if (!start)
    return 1;

  goal = searchVertex(g, goalStr);
  if (!goal)
    return 1;

  aStar(g, start, goal);
  delGraph(g);
}


List *createGraphFromFile(FILE *file) {
  char* line;
  size_t len;
  size_t read;
  char value1[100];
  int value2;
  int value3;
  List *g;
  Vertex *v1, *v2;
  Neighbour *n;
  
  g = newGraph();
  while ((read = getline(&line, &len, file)) != -1) {
    int entriesRead = sscanf(line, "%s %d %d", value1, &value2, &value3);
    if (entriesRead == 3) {
      v1 = searchVertex(g, value1);
      if (!v1) {
        v1 = newVertex(strdup(value1));
        addVertex(g, v1);
      }
      v1->lat = value2;
      v1->lng = value3;
    } else if (entriesRead == 2) {
      v2 = searchVertex(g, value1);

      if (!v2) {
        v2 = newVertex(strdup(value1));
        addVertex(g, v2);
      }

      n = newNeighbour();
      n->dist = value2;
      n->vertex = v2;
      addNeighbour(v1, n);
    }
  }
  return g;
}
