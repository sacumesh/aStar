#include "List.h"
#include "status.h"
#include <stdio.h>

List *newList(compFun comp, prFun pr) {
  List *res;

  res = (List *)malloc(sizeof(List));
  if (!res)
    return 0;

  res->head = 0;
  res->nelts = 0;
  res->comp = comp;
  res->pr = pr;

  return res;
}

void delList(List *l) {
  Node *tmp;

  while (l->head) {
    tmp = l->head;
    l->head = l->head->next;

    free(tmp);
  }

  free(l);
}

status nthInList(List *l, int n, void *res) {
  Node *tmp;

  if (n > l->nelts || n < 1)
    return ERRINDEX;

  tmp = l->head;
  while (--n)
    tmp = tmp->next;

  *(void **)res = tmp->val;

  return OK;
}

status addListAt(List *l, int n, void *e) {
  Node *tmp, *node;

  if (n > l->nelts + 1 || n < 1)
    return ERRINDEX;

  node = (Node *)malloc(sizeof(Node));
  if (!node)
    return ERRALLOC;

  if (n == 1) {
    node->next = l->head;
    l->head = node;
  } else {
    tmp = l->head;
    while (--n - 1)
      tmp = tmp->next;

    node->next = tmp->next;
    tmp->next = node;
  }

  node->val = e;
  ++l->nelts;

  return OK;
}

status remFromListAt(List *l, int n, void *res) {
  Node *tmp, *tmp1;

  if (n > l->nelts || n < 1)
    return ERRINDEX;

  if (n == 1) {
    tmp = l->head;
    l->head = tmp->next;
  } else {
    tmp1 = l->head;
    while (--n - 1)
      tmp1 = tmp1->next;

    tmp = tmp1->next;
    tmp1->next = tmp1->next->next;
  }
  --l->nelts;
  *(void **)res = tmp->val;
  free(tmp);

  return OK;
}

// removes all occurences of e in the list
status remFromList(List *l, void *e) {
  Node *tmp, *tmp1;
  status res;

  if (!l->comp)
    return ERRUNABLE;
  if (!l->head)
    return ERRABSENT;

  res = ERRABSENT;
  tmp = l->head;
  // remove all occuureces of e except if e is the first element
  while (tmp->next) {
    if (!(*l->comp)(tmp->next->val, e)) {
      tmp1 = tmp->next;
      tmp->next = tmp1->next;
      --l->nelts;
      free(tmp1);
      res = OK;
    }

    tmp = tmp->next;
  }

  // remove e if it is the first element
  if (!(*l->comp)(l->head->val, e)) {
    tmp = l->head;
    l->head = tmp->next;
    --l->nelts;
    free(tmp);

    res = OK;
  }

  return res;
}

status displayList(List *l) {
  Node *tmp;
  if (!l->pr)
    return ERRUNABLE;
  tmp = l->head;

  printf("[ ");
  while (tmp) {
    (*l->pr)(tmp->val);
    tmp = tmp->next;

    if (tmp)
      printf(", ");
  }
  printf(" ]");

  return OK;
}

void forEach(List *l, void (*f)(void *)) {
  Node *tmp;

  if (!f)
    return;

  tmp = l->head;
  while (tmp) {
    (*f)(tmp->val);
    tmp = tmp->next;
  }
}

status addList(List *l, void *e) {
  Node *node, *tmp;

  if (!l->comp)
    return ERRUNABLE;

  node = (Node *)malloc(sizeof(Node));
  if (!node)
    return ERRALLOC;

  if (!l->head || (*l->comp)(l->head->val, e) >= 0) {
    node->next = l->head;
    l->head = node;
  } else {
    tmp = l->head;
    while (tmp->next && (*l->comp)(tmp->next->val, e) <= 0) {
      tmp = tmp->next;
    }

    node->next = tmp->next;
    tmp->next = node;
  }

  node->val = e;
  ++l->nelts;

  return OK;
}

Node *isInList(List *l, void *e) {
  Node *tmp;

  if (!l->head)
    return 0;

  if (!(*l->comp)(l->head->val, e))
    return (Node *)1;

  tmp = l->head;
  while (tmp->next) {
    if (!(*l->comp)(tmp->next->val, e))
      return tmp;
    tmp = tmp->next;
  }

  return 0;
}

void *firstThat(List *l, int (*f)(void *)) {
  Node *tmp;

  tmp = l->head;
  while (tmp) {
    if ((*f)(tmp->val))
      return tmp->val;
    tmp = tmp->next;
  }

  return 0;
}

List *allThat(List *l, int (*f)(void *)) {
  List *res;
  Node *tmp;

  // if the creation of new list fails return 0
  res = newList(l->comp, l->pr);
  if (!res)
    return 0;

  tmp = l->head;
  while (tmp) {
    if ((*f)(tmp->val))
      addList(res, tmp->val);

    tmp = tmp->next;
  }

  return res;
}

int lengthList(List *l) { return l->nelts; }