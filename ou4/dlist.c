/*
 * File:                dlist.c
 * Author:              Elias Niko
 * CS-username:         c18eno@cs.umu.se
 * Date:                25-10-2019
 * Description: a double liked list for mfind to store the data.
*/

#include "dlist.h"

dlist *dlist_empty(void){
  dlist *l = calloc(1, sizeof(dlist));
  if (l == NULL) {
        perror("calloc");
  }
  if (l != NULL){
    cell *head = calloc(1, sizeof(cell));
    if (head == NULL) {
          perror("calloc");
    }
    l->head = head;
    l->length = 0;
    l->freeFunc = NULL;
    head->next = head;
    head->previous = head;
    head->val = NULL;
  }
  return l;
}

void dlist_memory_check(dlist *l, memFreeFunc *f){
  l->freeFunc = f;
}

dlist_pos dlist_insert(data val, dlist_pos p, dlist *l){
  dlist_pos pos = malloc(sizeof(cell));
  if (pos == NULL) {
        perror("malloc");
  }
  pos->val = val;
  pos->next = p;
  pos->previous = p->previous;
  p->previous->next = pos;
  p->previous = pos;
  l->length++;
  return pos;
}


bool dlist_is_empty(dlist *l){
  return !(l->length);
}


data dlist_inspect(dlist_pos p, dlist *l){
  if (p != l->head){
   return p->val;
  }
  return NULL;
}

dlist_pos dlist_first(dlist *l){
  return l->head->next;
}

dlist_pos dlist_end(dlist *l){
  return l->head->previous;
}

dlist_pos dlist_next(dlist_pos p, dlist *l){
    if (p != dlist_end(l)){
      return p->next;
    }
    return (dlist_pos)l->head;
}

dlist_pos dlist_pre(dlist_pos p, dlist *l){
  if (p != dlist_first(l)){
    return p->previous;
  }
  return (dlist_pos)l->head;
}

dlist_pos dlist_remove(dlist_pos p, dlist *l){
  dlist_pos returnPos = p->next;
    dlist_pre(p,l)->next = p->next;
    dlist_next(p,l)->previous = p->previous;


  if (l->freeFunc != NULL){
    l->freeFunc(p->val);
  }

  free(p);
  l->length--;
  if(returnPos == l->head){
    return l->head->previous;
  }
  return returnPos;
}

void dlist_kill(dlist *l){
  dlist_pos pos;
  while(!dlist_is_empty(l)){
    pos = dlist_first(l);
    dlist_remove(pos,l);
  }
  free(l->head);
  free(l);
}
