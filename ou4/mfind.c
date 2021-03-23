/*
 * File:                mfind.c
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
#include "mfind.h"


dlist *thread_buffer;

int main(int argc, char *argv[]) {
    dlist *directory_list = dlist_empty();
    dlist *path = dlist_empty();
    thread_buffer = dlist_empty();
    dlist_memory_check(path, free);
    dlist_memory_check(thread_buffer, free);
    pthread_t *thrID = NULL;
    input_to_search ui;
    int sleep_thr = 0;
    int returnValue;
    int *ret;
    int ret1;

    if(pthread_mutex_init( & mutex_path, NULL)){
          perror("pthread_mutex_init");
          exit(EXIT_FAILURE);
   }

    if (pthread_mutex_init( & mutex_directory, NULL)) {
          perror("pthread_mutex_init");
          exit(EXIT_FAILURE);
    }
    if (pthread_cond_init( & wait, NULL)) {
          perror("pthread_cond_init");
          exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init( & mutex_thread, NULL)) {
          perror("pthread_mutex_init");
          exit(EXIT_FAILURE);
    }

    returnValue = get_input(argc, argv, directory_list, &ui);

    if (!returnValue) {
        state = SEARCHING;
        info_to_search *sinfo = search_info(ui, directory_list,
            path, & sleep_thr);
        returnValue = controll_start_dir(sinfo);
        thrID = create_thr(ui, directory_list, path, &sleep_thr);
        if ((ret = (int *) search_dir(sinfo)))
            returnValue = *ret;
        free(ret);
        if ((ret1 = wait_for_threads(ui.nrthr, thrID)))
            returnValue = ret1;
        print_result(path);
        printf("\n");
        print_result(thread_buffer);
    }
    if (pthread_mutex_destroy(&mutex_directory)) {
          perror("pthread_mutex_destroy");
          exit(EXIT_FAILURE);
    }
    if (pthread_mutex_destroy(&mutex_path)) {
          perror("pthread_mutex_destroy");
          exit(EXIT_FAILURE);
    }
    if (pthread_cond_destroy(&wait)) {
          perror("pthread_mutex_destroy");
          exit(EXIT_FAILURE);
    }
    if (pthread_mutex_destroy(&mutex_thread)) {
          perror("pthread_mutex_destroy");
          exit(EXIT_FAILURE);
    }

    dlist_memory_check(directory_list, free);
    dlist_memory_check(thread_buffer, free);
    dlist_kill(thread_buffer);
    dlist_kill(directory_list);
    dlist_kill(path);
    free(thrID);

    return returnValue;

}

int get_input(int argc, char *argv[], dlist *directory_list,
              input_to_search *input) {
    input-> nrthr = 1;
    input-> ftype = 0;
    input-> filename = NULL;
    bool tflag = false, pflag = false;
    int c, i;
    int invalidArg = 0;

    while ((c = getopt(argc, argv, "t:p:")) != -1) {
        switch (c) {
        case 't':
            if (strlen(optarg) > 1 || tflag || (optarg[0] != 'l' &&
                    optarg[0] != 'd' && optarg[0] != 'f')) {
                fprintf(stderr, "Error: Invalid argument for -t:\n"
                    "[-t d] to directory.\n"
                    "[-t f] to regular file.\n"
                    "[-t l] to symbolic link.\n");
                invalidArg = 1;
            }
            input-> ftype = optarg[0];
            tflag = true;
            break;
        case 'p':
            i = 0;
            do {
                if (pflag || (!isdigit(optarg[i]))) {
                    fprintf(stderr, "Invalid argument for option -p:"
                        " The argument following -p flag must"
                        " be an integer.\n");
                    invalidArg = 1;
                }
                i++;
            } while (optarg[i] != '\0');
            pflag = true;
            input-> nrthr = atoi(optarg);
            break;
        default:
            invalidArg = 1;
            break;
        }
    }
    while (optind < argc - 1) {
        char *arg = malloc((strlen(argv[optind]) + 1) * sizeof(char));
        if (arg == NULL) {
             perror("malloc()");
        }
        strncpy(arg, argv[optind], strlen(argv[optind]) + 1);
        if ((strlen(arg) > 1) && (arg[strlen(arg) - 1] == '/'))
            arg[strlen(arg) - 1] = '\0';
        dlist_insert(arg, dlist_end(directory_list), directory_list);
        optind++;
    }
    if (dlist_is_empty(directory_list) || invalidArg) {
        fprintf(stderr, "USAGE: $./mfind [-t type]"
                  "[-p nrthr] start1 [start2...] name\n");
        return 1;
    }
    input-> filename = argv[optind];
    return 0;
}

int controll_start_dir(info_to_search * info) {
    struct stat lstat_buffer;
    char * file_adress, * str;
    char * dir = "";
    dlist_pos pos = NULL;
    pos = dlist_first(info-> directory_list);

    // Iterate through the list and check for matches
    do {
        file_adress = (char * ) dlist_inspect(pos, info-> directory_list);
        if (lstat(file_adress, & lstat_buffer)) {
            perror("Error: lstat()");
            return errno;
        }

        //Modify the strings to get correct format.
        if ((dir = strstr(file_adress, info-> name))) {
            int length = strlen(file_adress) - strlen(info-> name);
            str = calloc(length, sizeof(char));
            if (str == NULL) {
                perror("Error: calloc()");
                exit(EXIT_FAILURE);
            }
            strncpy(str, file_adress, (length - 1));
            file_status(info, & lstat_buffer, dir, str);
            free(str);
        }
        pos = dlist_next(pos, info-> directory_list);
    } while (dlist_inspect(pos, info-> directory_list));
    return 0;
}

pthread_t * create_thr(input_to_search ui, dlist * directory_list,
                        dlist * search_path, int * sleep_thr) {
    pthread_t * thrID = NULL;
    int err;
    info_to_search * sinfo = NULL;
    // store thread ID's in an array
    if (ui.nrthr > 1) {
        thrID = malloc((ui.nrthr) * sizeof(pthread_t));
        if (thrID == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    } else return NULL;
    thrID[0] = pthread_self();
    // create threads
    if (thrID != NULL) {
        for (int t = 1; t < ui.nrthr; t++) {
            sinfo = search_info(ui, directory_list, search_path, sleep_thr);
            if ((err = pthread_create( & thrID[t], NULL, search_dir, sinfo))) {
                // fprintf(stderr,"Error: creating thr %d,""error = %d\n", t, err);
                perror("pthread_create:");
                exit(EXIT_FAILURE);
            }
        }
    }
    return thrID;
}

info_to_search * search_info(input_to_search ui, dlist * directory_list,
                              dlist * search_path, int * sleep_thr) {
    info_to_search * sinfo = malloc(sizeof(info_to_search));
    if (sinfo == NULL)
        perror("Error: malloc()");
    sinfo-> name = ui.filename;
    sinfo-> type = ui.ftype;
    sinfo-> counter = 0;
    sinfo-> nrthr = ui.nrthr;
    sinfo-> directory_list = directory_list;
    sinfo-> path = search_path;
    sinfo-> sleep_thr = sleep_thr;

    return sinfo;
}

void *search_dir(void * args) {

    info_to_search * info = args;
    int * returnValue = malloc(sizeof(int));
    int ret = 0;
    char * dir = NULL;
    if (returnValue == NULL){
          perror("Error: malloc()");
          state = DONE;
          return (void * ) EXIT_FAILURE;
   }

    *returnValue = 0;
    while (state == SEARCHING) {
          dir = NULL;
          pthread_mutex_lock( & mutex_directory);

          if (dlist_is_empty(info-> directory_list)) {
             if ( * (info-> sleep_thr) >= info-> nrthr - 1) {
                  state = DONE;
                  pthread_cond_broadcast( & wait);
             }
             while(dlist_is_empty(info->directory_list) && state == SEARCHING){
                  *(info-> sleep_thr) += 1;
                  pthread_cond_wait(&wait, &mutex_directory);
                  *(info-> sleep_thr) -= 1;
             }
          }
          //If list is not empty, collect directory.
          if (!dlist_is_empty(info-> directory_list)) {
             dir = dlist_inspect(dlist_first(info-> directory_list),
                                      info-> directory_list);
             dlist_remove(dlist_first(info-> directory_list),
                                      info-> directory_list);
          }

          pthread_mutex_unlock( & mutex_directory);

          if(dir == NULL){
               continue;
               exit(1);
          }
          else {
            ret = search_for_file(info, dir);
            free(dir);
            if (ret)
                  * returnValue = ret;
        }
   }
    print_info(info);
    free(info);
    return returnValue;
}

void print_info(info_to_search * sinfo) {
      char *result = calloc(50, sizeof(char));
      if (result == NULL)
            perror("calloc()");
      unsigned long c = pthread_self();
      sprintf(result,"Thread: %lu Reads: %d", c, sinfo-> counter);
      pthread_mutex_lock(&mutex_thread);
      dlist_insert(result, dlist_first(thread_buffer), thread_buffer);
      pthread_mutex_unlock(&mutex_thread);
}

int wait_for_threads(int nrthr, pthread_t * thrID) {
    pthread_t self = pthread_self();
    void * threadReturn = NULL;
    int ret = 0;
    int t = 0;
    while ((nrthr > 1) && (t < nrthr)) {
        if (!(thrID[t] == self)) {
            pthread_join(thrID[t], & threadReturn);
            if (threadReturn != NULL && * (int * ) threadReturn != 0)
                ret = * (int * ) threadReturn;
            free(threadReturn);
            threadReturn = NULL;
        }
        t++;
    }
    return ret;
}

int search_for_file(info_to_search * info, char * dir) {

    DIR * d;
    struct dirent * currentDir;
    struct stat lstat_buffer;
    char * filepath;
    int returnValue = 0;
    info-> counter++;
    if ((d = opendir(dir)) == NULL) {
        perror(dir);
        return 0;
    }

    while ((currentDir = readdir(d)) != NULL) {
          if (currentDir == NULL) {
                perror("readdir");
                exit(EXIT_FAILURE);
          }
        filepath = cat_strings(dir, currentDir-> d_name);
        if (lstat(filepath, & lstat_buffer)) {
            perror("lstat");
            returnValue = errno;
        }
        insert_dir_to_list( & lstat_buffer, currentDir, info, dir);
        file_status(info, & lstat_buffer, currentDir-> d_name, dir);
        free(filepath);
    }
    closedir(d);
    return returnValue;

}

void file_status(info_to_search * info, struct stat * lstat_buffer,
    char * currentFile, char * dir) {
    switch (info-> type) {
    case 'd':
        if (S_ISDIR(lstat_buffer-> st_mode)) {
            if (strcmp(info-> name, currentFile) == 0) {
                insert_path_to_list(info, dir);
            }
        }
        break;
    case 'f':
        if (S_ISREG(lstat_buffer-> st_mode)) {
            if (strcmp(info-> name, currentFile) == 0) {
                insert_path_to_list(info, dir);
            }
        }
        break;
    case 'l':
        if (S_ISLNK(lstat_buffer-> st_mode)) {
            if (strcmp(info-> name, currentFile) == 0) {
                insert_path_to_list(info, dir);
            }
        }
        break;
        // no file type by user
    default:
        if (strcmp(info-> name, currentFile) == 0) {
            insert_path_to_list(info, dir);
        }
        break;
    }
}

void insert_dir_to_list(struct stat * lstat_buffer, struct dirent * currentDir,
    info_to_search * info, char * filepath) {
    char * str;
    if ((S_ISDIR(lstat_buffer-> st_mode))) {
        if (strcmp(".", currentDir-> d_name) != 0 &&
            strcmp("..", currentDir-> d_name) != 0) {
            str = cat_strings(filepath, currentDir-> d_name);
            pthread_mutex_lock( & mutex_directory);
            dlist_insert(str, dlist_end(info-> directory_list), info-> directory_list);
            pthread_cond_broadcast( & wait);
            pthread_mutex_unlock( & mutex_directory);
        }

    }
}

void insert_path_to_list(info_to_search * info, char * filepath) {
    char * str;
    str = cat_strings(filepath, info-> name);
    pthread_mutex_lock( & mutex_path);
    dlist_insert(str, dlist_first(info-> path), info-> path);
    pthread_mutex_unlock( & mutex_path);

}

char * cat_strings(char * str1, char * str2) {
    int strSize = (strlen(str1) + strlen(str2)) + 2;
    char * str = malloc(strSize * sizeof(char));
    if (str == NULL) {
        perror("Error: malloc()");
        exit(EXIT_FAILURE);
    }

    strcpy(str, str1);
    if (strlen(str) > 1) {
        strcat(str, "/");
    }
    strcat(str, str2);
    return str;
}

void print_result(dlist * l) {
    char * s_path;
    while (!dlist_is_empty(l)) {
        s_path = dlist_inspect(dlist_first(l), l);
        fprintf(stdout, "%s\n", s_path);
        dlist_remove(dlist_first(l), l);
    }
}
