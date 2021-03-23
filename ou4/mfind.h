/*
 * File:                mfind.h
 * Author:              Elias Niko
 * CS-username:         c18eno@cs.umu.se
 * Date:                25-10-2019
 * Description: mfind is like unix "find" command. it serachs after a given
 * file and directory and prints out the result for user in terminal,
 * mfind also prints amout of read for each thread.
 *
 *
 *
 *
 * USAGE: ./mfind [-t type] [-p nrthr] start1 [start2 ...] name
*/


#ifndef __mfind
#define __mfind

#define _DEFAULT_SOURCE
#define DONE 1
#define SEARCHING 0


#include <pthread.h>
#include <stdio.h>
#include "dlist.h"
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>

typedef struct {
    dlist *directory_list;
    dlist *path;
    int counter;
    char type;
    char *name;
    int nrthr;
    int *sleep_thr;

}info_to_search;

typedef struct {
    int nrthr;
    char ftype;
    char *filename;
}input_to_search;


unsigned int state;
pthread_cond_t wait;
pthread_mutex_t mutex_directory, mutex_path, mutex_thread;



/*
 *
 *  Parse the program arguments and controlls
 *      that the format is correct.
 *
 *  @params : argc - number of arguments
 *            argv - array of strings, containing user arguments.
 *            *input   - a struct to store information about program arguments.
 *            *dirrectory_list - pointer to list
 *
 *  @Return: Returns zero if arguments are incorrect.
 *
 */
int get_input(int argc, char *argv[], dlist *directory_list, input_to_search *input);

/*
*
*
* Will check if the target file is one of the starting
* directoties. If so will add them to the list of found searchpaths.
*
* @param: *info - contains search information.

*/

int controll_start_dir(info_to_search *info);

/**
 * Check and creates how many threads create.
 *      Will also set up a barrier for synchronization.
 *      Stores all thread ids in a global variabele(threads).
 *
 *  @param: input - struct contaning user input.
 *              *directory_list  - pointer to directory listi
 *              *search_path - pointer to search_path list.
 *              *sleep_thr - pointer to interger.
 */

pthread_t *create_thr(input_to_search input, dlist *directory_list, dlist *search_path,
        int *sleep_thr);
  /**
   * Allocates memory for a searchInfo
   *  and returns the pointer.
   *
   *  @param: unput - user input
   *             *directory_list - pointer to directory list
   *             *search_path - pointer to search_path list
   *             *sleep_thr - pointer to interger
   *
   *  @Return: Pointer to a info_to_search
   */
info_to_search *search_info(input_to_search input, dlist *directory_list,
        dlist *search_path, int *sleep_thr);
  /*
   *
   *  Collect a directory from the list of directories. When and if a
   *  directory is collected, it will conduct a search for the target file.
   *  When the search is complete it will deallocate its argument and return 0.
   *
   *  @param: *args -  void pointer to searchInfo struct.
   */

void *search_dir(void *args);

/*
   Is called by "main" thread, will wait for all other threads to
 * finish their work.
 *
 * @param: thrID -  integer of how many threads were created.
 *
 * @Return: Will return 0 if no thread encountered an error, else some error
 */
int wait_for_threads(int nrthr, pthread_t *thrID);
/*
 * Function searches an directory specified by *dir, it will store
 *  new directories in directory list. It will also check if any
 *  files/directories/link match with the target file.
 *
 *  Paramters: *indfo - struct containing search information
 *             *dirr - string of a directory.
 *
 *  Return: 0 if all Ok, else some error number !=0
 */
int search_for_file(info_to_search *info, char *dirr);

/*
 * Will control each file, given by the cur_file argument if
 * their name and file type match with the target file. If so calls
 * addSearchPath(), which will add it in the given list.
 *
 * @param: *info - containing search information
 *			   *lstat_buffer - struct with information form lstat
 *			   *cur_file - name of the current file
 *			   *dir - name of current directory.
 *
 * @Return: 0 if all Ok, else some error value != 0.
 */
void file_status(info_to_search *info, struct stat *lstat_buffer,
        char *cur_file, char *dir);

  /*
   * Checks if the current file is a directory, if so adds that
   * directory in the directory list. Does not add directory "." and "..".
   *
   * @param: *lstat_buffer -  "return" value from lstat, information about file.
   *             *current_dir - return value from readDir, information about file.
   *             *si - struct containing seach information
   *             *filepath - Searchpath to current directory.
   *
   */

void insert_dir_to_list(struct stat *lstat_buffer, struct dirent *current_dir,
        info_to_search *info, char *filepath);
  /*
   * If a search_path to a file is found, add search_path to the list
   * containing all searchpaths to current file.
   *
   * @param: *si - struct containing search information
   *             *filepath - search_path to current directory.
   *
   */
void insert_path_to_list(info_to_search *info, char *filepath);
/*
 *  Function will concatenate two strings into one. It will allocate
 *  memory for a new string and use strcat to merge the two string arguments. It
 *  will add an "/" between the two arguments.
 *
 * @param: *str1 - the "first" part of the new string.
 *             *str2  - the "other" part of the new string
 *
 * @Return: A string were dest and src have been merged together.
 */
char *cat_strings(char *dest, char *src);

/*
 * Prints out the all searchpaths were the target file was found.
 *
 *  @param: *l - list of strings.
 */
void print_result(dlist *l);
/*
 * Prints out all info_to_search struct.
 *
 *  @param: *sinfo - struct info_to_search.
 */

void print_info(info_to_search *sinfo);

#endif
