#include "List.h"
#include "status.h"



List* newList(compFun comp,prFun pr){
    List * res = (List *) malloc(sizeof(List));

    if(!res) return 0;

    res -> head = 0;
    res -> nelts = 0;
    res -> comp = comp;
    res -> pr = pr;

    return res;
}	


void delList(List* l) {
    Node * tmp;
    while (l -> head)
    {
        tmp = l -> head;
        l -> head = l -> head -> next;
        free(tmp);
    }

    free(l);
}

