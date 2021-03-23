/*
 * File:                      list.h
 * Authur:                    Elias Niko
 * Email:                     c18eno@cs.umu.se
 * Current version: 1.1
 * Date:                      2019-10-10
*/

#ifndef __DLIST_H
#define __DLIST_H

#include <stdbool.h>

typedef struct list list;

typedef struct node *node;



list *list_empty(void);
node list_next(const node p);
bool list_is_empty(const list *l);
node list_first(const list *l);
void change(struct node *a, struct node *b);
bool list_is_end(const node p);
void list_kill(list *l);
char *list_inspect_uname(const node p);
void *list_inspect(const node n);
void list_insert(char *s, unsigned int i, const node p);
void list_remove(const node p);
unsigned int list_inspect_uid(const node p);

 #endif
