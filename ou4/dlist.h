/*
 * File:                dlist.h
 * Author:              Elias Niko
 * CS-username:         c18eno@cs.umu.se
 * Date:                25-10-2019
 * Description: header file of a double directed list
 * for mfind to store the data.
 *
*/

#include <errno.h>
#ifndef __LIST__
#define __LIST__
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#ifndef __DATA__
#define __DATA__
typedef void *data;
#endif

#ifndef __FREEFUNC__
#define __FREEFUNC__
typedef void memFreeFunc(data);
#endif

/*
 * structure for element in the list
*/
typedef struct cell {
  struct cell *previous;
  data val;
  struct cell *next;
} cell;

typedef cell *dlist_pos;

/*
 * structure for double liked list
*/
typedef struct{
  cell *head;
  memFreeFunc *freeFunc;
  int length;
} dlist;

/*
  Create a dllist


  @Return: Pointer to the dllist wich ahve been created.
*/

dlist *dlist_empty(void);

/*
 * handle the memroy for those element in dllist which is not
 * longer in the dllist.
 * @param: l: a linked list
           f: function that deallocates memory.
*/

void dlist_memory_check(dlist *l, memFreeFunc *f);

/*
 * insert an elemnt before the given position in the listwith the given value.
 * @param: *value pointer to the return
 *         p position
           *l pointer to the double linkedlist
 * @Return: The position of the new element as an integer.
*/

dlist_pos dlist_insert(data val, dlist_pos p, dlist *l);

/*
  Checks if the given list if empty.
  @Returns : True if empty, false otherwise.
  @param: l - pointer to a doublelinked list.
*/

bool dlist_is_empty(dlist *l);

/*
  Inspects and returns the value on the given position in the list.
  @param: *p - pointer to the position
          *l - pointer to the list
  @Return: data - pointer to the curernt value on the given position.
*/

data dlist_inspect(dlist_pos p, dlist *l);

/*
  Returns the position for the first element for the given list.
  @param: l - pointer to a doublelinked list.
  @Return: The position of the first element in the list.
*/

dlist_pos dlist_first(dlist *l);

/*
  Returns the position for the last element for the given list.
  @param: l - pointer to a doublelinked list.
  @Return: The position of the last element in the list.
*/

dlist_pos dlist_end(dlist *l);

/*
  This function returns a pointer to the next element in
  the doublelinked list.
  @param: p - pointer a position in the list
          l - pointer to the doublelinked list
  @Return: Pointer to the next position in the given list
*/

dlist_pos dlist_next(dlist_pos p, dlist *l);

/*
  This function returns a pointer to the previous
  element in the doublelinked list.
  @param: p - pointer a position in the list
          l - pointer to the doublelinked list
  @Return: Pointer to the previous position in the given list
          but will return NULL if on first element
*/

dlist_pos dlist_pre(dlist_pos p, dlist *l);

/*
  Removes the element on the given position and returns the position for the
  following element.
  @param:   *p - pointer to given position in the doublelinked list
            *l - pointer to given list
  @Return: The position to the element following the removed element.
           Retuns NULL if list is empty and if list becomes empty after
           removing an element.
*/

dlist_pos dlist_remove(dlist_pos p, dlist *l);

/*
  Removes the list and deallocates all memory.
  Parameters :  l -  A pointer to the given doublelinked list.
*/

void dlist_kill(dlist *l);

#endif
