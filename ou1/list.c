/*
 * File:                      list.c
 * Authur:                    Elias Niko
 * Email:                     c18eno@cs.umu.se
 * Current version: 1.1
 * Date:                      2019-10-10
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <string.h>


/*
 * these struct's are from specification ou1
*/

struct user_info{
  unsigned int uid;
  char* uname;
};

struct node {
  void *value;
	struct node *next;
};

struct list {
	struct node *head;
};



/*
  * Create an empty list by allocating memory for the list and the head.
*/
list *list_empty(void){
  list *l = calloc(1, sizeof(*l));
  if(!l){
    fprintf(stderr,"Memory's not allocated.\n");
    exit(EXIT_FAILURE);
  }
  l->head = calloc(1,sizeof(struct node));
  if(!l->head){
        fprintf(stderr,"Memory's not allocated.\n");
    exit(EXIT_FAILURE);
  }
  l->head->next = NULL;
  return l;
}

/*
  * Checks if a list is empty.
*/
bool list_is_empty(const list *l){
  if(l->head->next == NULL){
    return true;
  }
  else{
    return false;
  }
}

/*
  * Input: a pointer to the list
  * Output: a pointer to the first node of the list.
*/
node list_first(const list *l){
  return l->head;
}

/*
  * Input: a node in a list
  * Output: a pointer to the next node in the same list.
*/
node list_next(const node n){
  return n->next;
}


/*
  * Checks if a node is the last node in the list.
*/
bool list_is_end(const node n){
  if(n->next == NULL){
    return true;
  }
  else{
    return false;
  }
}

/*
  * Input: node to be inspected
  * Output: The number which is saved in the user_info.
*/
unsigned int list_inspect_uid(const node n){
  if(!list_is_end(n)){
    struct user_info *user = n->next->value;
    return user->uid;
  }
  else{
    return -1;
  }
}


/*
  * Input: node to be inspected
  * Output: a pointer to the string which is saved in the user_info
*/
char *list_inspect_uname(const node n){
  if(!list_is_end(n)){
    struct user_info *user = n->next->value;
    return user->uname;
  }
  else{
    return NULL;
  }
}


/*
  * Input: node to be inspected
  * Output The value of a node (user_info).
*/

void *list_inspect(const node n){
  if(list_is_end(n)){
    fprintf(stderr,"Warning: End of the list\n");
  }
  return n->next->value;
}



/*
  * Inserts the data to the list
*/
void list_insert(char *s, unsigned int i, const node n){
  node new_node = calloc(1, sizeof(struct node));
  if(!new_node){
    fprintf(stderr,"Memory's not allocated.\n");
    exit(EXIT_FAILURE);
  }

  struct user_info *user = calloc(1, sizeof(struct user_info));
  if(!user){
    fprintf(stderr,"Memory's not allocated.\n");
    exit(EXIT_FAILURE);
  }

  char *v = calloc(32, sizeof(char));
  if(!v){
    fprintf(stderr,"Memory's not allocated.\n");
    exit(EXIT_FAILURE);
  }
  user->uname = v;
  strcpy(v, s);
  user->uid = i;
  new_node->value = user;
  new_node->next = n->next;
  n->next = new_node;
}

/*
  * Remove the data from the node
*/
void list_remove(const node n){
  node d = n->next;
  n->next = d->next;
  if(d->value != NULL){
    struct user_info *user = d->value;
    free(user->uname);
    free(d->value);
  }
  free(d);
}

/*
 * Changes the position of a to b's position.
*/
void change(struct node *a, struct node *b){
  struct user_info *temp = a->next->value;
  a->next->value = b->next->value;
  b->next->value = temp;
}

/*
  * kill all allocated memories, and removes all data from the list
*/
void list_kill(list *l){
  node n = list_first(l);
  while(!list_is_empty(l)){
    list_remove(n);
  }
  free(l->head);
  free(l);
}
