/*
 * File:      mpasswdsort.c
 * Authur:    Elias Niko
 * Email:     c18eno@cs.umu.se
 * Current version: 1.1
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <error.h>
#include "list.h"

#define BUFFSIZE 1023
#define PASSW_BUFSIZE 32
#define BUFF_UID 32

void read(int argc, char const **argv, list *list, int *error);
char *uname_check(const char *s, int line_number, int *error);
bool gid_check(const char *s, int line_number, int *error);
bool gid_number_check(const char *s);
bool invalid_line(const char *s, int line_number, int *error);
bool check_if_colon(char c);
int non_white_spc(const char *s);
bool line_is_blank(const char *s, int line_number, int *error);
char *get_uid (const char *s);
bool hme_field_check(const char *s, int line_number, int *error);
bool uid_check(const char *s, int line_number, int *error);
unsigned int str_num(char *s);
list *bubble_sort(list *lst);


/*
  *  Find the first element in array wich is charachter.
  @s char s
*/


int non_white_spc(const char *s){
      int i = 0;
      while(s[i] && isspace(s[i])){
            i++;
      }
      if(s[i])
            return i;
      else  return -1;
}

/*
  *  Checks if a specific line is blank or not.
  *
  * @s  constant char string
  * @s   line nuber.
  * Return: true if a line is blank false if the line is not blank.
*/
bool line_is_blank(const char *s, int line_number, int *error){
      if(non_white_spc(s) < 0){
            fprintf(stderr, "Line %d: Encountered a <BLANKLINE>\n", line_number);
            *error = 1;
            return true;
      }
      else return false;
}

/*
  *  checks if the specific charachter is colon.
  *
  * @s  char s
  * Return: true if the charachter is colon, false if not.
*/
bool check_if_colon(char c){
      return(c == ':');
}


/*
  *  checks if a line indicates invalid parameters.
  *
  * @s   constant char string pointer to s
  * @s   line number
  * Return: true if the line is blank, otherwise false.
*/
bool invalid_line(const char *s, int line_number, int *error){
      int i = 0, index = 0;
      while(s[i] && s[i] != '\0'){
            if(check_if_colon(s[i])){
                  index++;
            }
            i++;
      }
      if(index < 6){
            fprintf(stderr, "Line %d: Invalid format: %s", line_number, s);
            *error = 1;
            return true;
      }
      else return false;
}


/*
  *  checks if a characeter is uid.
  * @s   char string s as a pointer
  * Return pointer to uid
*/

char *get_uid (const char *s){
      int index = 0, i = 0;
      while(s[i] && index != 2){
            if(check_if_colon(s[i])){
                  index++;
            }
            i++;
      }
      char *user_id = calloc(PASSW_BUFSIZE,sizeof(char));
      if(!user_id){
            fprintf(stderr,"Memory not allocated.\n");
            exit(EXIT_FAILURE);
      }
      int counter = 0;
      while(!check_if_colon(s[i])){
            user_id[counter] = s[i];
            i++;
            counter++;
      }
      user_id[counter + 1] = '\0';
      return user_id;
}

/*
  *  when uid is done this function check if the uid indicates just number.
  *
  * @s  char string s
  * @s   line number
  * Return: returns true if the string are just numbers.
*/
bool uid_check(const char *s, int line_number, int *error){
      int i = 0;
      while(s[i] && isdigit(s[i])) i++;
      if(s[i] != '\0'){
            fprintf(stderr,"Line %d: UID has to be a number. Got: \"%s\" \n",
                  line_number, s);
            *error = 1;
            return false;
      }
      else if (i == 0){
            fprintf(stderr,"Line %d: The UID Field cannot be empty.\n",
                  line_number);
            *error = 1;
            return false;
  }
      else return true;
}

/*
  *  chenges a string charachter to number (integer)
  *
  * @s   char string s
  * Return the number
*/
unsigned int str_num(char *s){
      int number = 0, digit;
      char c;
      if(s[0] == '-'){
            for(unsigned int i = 1; i < strlen(s); i++){
                  c = s[i];
                  digit = c - '0';
                  number = number * 10 + digit;
            }
            return (number - (2 * number));
      }
      else{
            for(unsigned int i = 0; i < strlen(s); i++){
                  c = s[i];
                  digit = c - '0';
                  number = number * 10 + digit;
            }
            return number;
      }
}


/*
  *  checks if uid indicates is >= 0 or not empty.
  *
  * @s :  char string s
  * @s :  integer which indicates the number of line.
  * Return: returns true if gid is positive and non empty.
*/
bool gid_check(const char *s, int line_number, int *error){
      bool valid = true;
      int index = 0, i = 0;
      while(s[i] && index != 3){
            if(check_if_colon(s[i]))
                  index++;
            i++;
      }


      char *gid = (char*) calloc(PASSW_BUFSIZE,sizeof(char));
      if(!gid){
            fprintf(stderr,"Memory's not allocated.' \n");
            exit(EXIT_FAILURE);
      }
      int counter = 0;
      while(!check_if_colon(s[i])){
            gid[counter] = s[i];
            i++;
            counter++;
      }
      if(counter == 0){
            fprintf(stderr,"Line %d: The GID Field cannot be empty.\n",
                  line_number);
            *error = 1;
            valid = false;
      }
      else if(!gid_number_check(gid)){
            fprintf(stderr,"Line %d: GID has to be a number. Got %s\n",
                  line_number, gid);
            *error = 1;
            valid = false;
      }
      else{
            gid[counter + 1] = '\0';
            int gid_num = str_num(gid);
            if(gid_num < 0){
                  fprintf(stderr,"Line %d: GID has to be a positive"
                  " number. Got %d\n",
                  line_number, str_num(gid));
                  *error = 1;
                  valid = false;
            }
      }
      free(gid);
      return valid;
}

/*
  *  checks if GID is just number.
  *
  * @s :  char string s
  * Return: true if the GID is a number
*/
bool gid_number_check(const char *s){
      int i = 0;
      while(s[i] && isdigit(s[i]))
            i++;
      if(s[i] != '\0') return false;
      else return true;
}

/*
  *  The function checks if the user_name field is empty or not.
*/
char *uname_check(const char *s, int line_number, int *error){
      int i = non_white_spc(s);
      char *user_name = calloc(PASSW_BUFSIZE,sizeof(char));
      if(!user_name){
            fprintf(stderr,"Memory's not allocated.\n");
            exit(EXIT_FAILURE);
      }
      int counter = 0;
      while(s[i] && !check_if_colon(s[i])){
            user_name[counter] = s[i];
            i++;
            counter++;
      }
      if(user_name[0] == '\0'){
            fprintf(stderr, "Line %d: The user_name field cannot be empty.\n",
                  line_number);
            *error = 1;
            free(user_name);
            return NULL;
      }
      else return user_name;
}
/*
  *  checks if home field is valid.
*/
bool hme_field_check(const char *s, int line_number, int *error){
      int index = 0;
      bool valid = true;
      int i = 0;
      while(s[i] && index != 5){
            if(check_if_colon(s[i]))
                  index++;
            i++;
      }
      int counter = 0;
      while(s[i] && !check_if_colon(s[i])){
            if(!isspace(s[i]))
                  counter++;
            i++;
      }
      if(counter == 0 ){
            fprintf(stderr, "Line %d: The home field cannot be empty.\n",
                  line_number);
            *error = 1;
            valid = false;
      }
      return valid;
}



/*
  *  reads information from a file and checks the requirments.
*/
void read(int argc, char const **argv, list *lst, int *error){
        char line[BUFFSIZE], *user_id, *user_name;
        FILE *in;
        int counter = 1;
        bool input_file_opened = false;
        unsigned int uid_num;
        struct node *n = list_first(lst);
        if(argc > 1){
             in = fopen(argv[1], "r");
             if(in == NULL){
                   perror("Failed: ");
                   exit(EXIT_FAILURE);
             }
             input_file_opened = true;
       }
       else in = stdin;

       while(fgets(line,BUFFSIZE, in) != NULL){
             if(!line_is_blank(line, counter, error)){
                   if(!invalid_line(line, counter, error)){
                         if(hme_field_check(line,counter, error)
                              && gid_check(line,counter, error)){
                               user_id = get_uid(line);
                               if(uid_check(user_id, counter, error)){
                                     uid_num = str_num(user_id);
                                     user_name = uname_check(line,
                                                counter, error);
                                     if(user_name != NULL){
                                           list_insert(user_name, uid_num, n);
                                           n = list_next(n);
                                           free(user_name);
                                     }
                               }
                               free(user_id);
                         }
                   }
             }
             counter++;
       }
       if(input_file_opened){
             fclose(in);
       }
 }

/*
  *  sorts the list by bubblesorting. and returns the sorted list as pointer
*/
list *bubble_sort(list *lst){
      bool swapped;
      struct node *ptrl;
      unsigned int data1, data2;
      do{
            swapped = false;
            ptrl = list_first(lst);
            while(!list_is_end(ptrl)){
                  data1 = list_inspect_uid(ptrl);
                  data2 = list_inspect_uid(list_next(ptrl));
                  if(data1 > data2){
                       change(ptrl, list_next(ptrl));
                       swapped = true;
                 }
                 ptrl = list_next(ptrl);
           }
     }while(swapped);
     return lst;
}

int main(int argc, char const **argv){
      int number = 0;
      int *error = (int*) malloc(1 * sizeof(int));
      if(!error){
            fprintf(stderr,"Memory's not allocated\n");
            exit(EXIT_FAILURE);
      }
      *error = 0;
      list *lst = list_empty();
      read(argc, argv, lst, error);
      if(*error > 0) // got an error
      number = 1;
      lst = bubble_sort(lst);
      struct node *n = list_first(lst);
      while(!list_is_end(n)){
            printf("%d:%s\n", list_inspect_uid(n),
                  list_inspect_uname(n));
            n = list_next(n);
      }
      list_kill(lst);
      free(error);
      return number;
}
