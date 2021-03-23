/*
* File:                 execute.c
* Authur:               Elias Niko
* Email:                c18eno@cs.umu.se
* Current version:      1.0
* Date:                 2019-10-11
*/


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "execute.h"

/* Duplicate a pipe to a standard I/O file descriptor
 * Arguments:	pip	the pipe
 *		end	tells which end of the pipe shold be dup'ed; it can be
 *			one of READ_END or WRITE_END
 *		destfd	the standard I/O file descriptor to be replaced
 * Returns:	-1 on error, else destfd
 */
int dupPipe(int pip[2], int end, int destfd){
      int dup;
      dup = dup2(pip[end], destfd);
      if (dup < 0) {
            perror("Error: dup2()");
            return -1;
      }
      else return destfd;
}

/* Redirect a standard I/O file descriptor to a file
 * Arguments:	filename	the file to/from which the standard I/O file
 * 				descriptor should be redirected
 * 		flags	indicates whether the file should be opened for reading
 * 			or writing
 * 		destfd	the standard I/O file descriptor which shall be
 *			redirected
 * Returns:	-1 on error, else destfd
 */

int redirect(char *filename, int flags, int destfd){
      FILE *file;
      int filefd;

      // Check if stdout is already redirected to a files
      // check if the file already exists.
      // close file just when you open it for writing not reading.

      if(destfd == STDOUT_FILENO){
            if ((file = fopen(filename, "r"))){
                  errno = EEXIST;
                  return -1;
            }
      }

      if((filefd = open(filename, flags)) < 0)
            return -1;
            
      int red_dup2;
      red_dup2 = dup2(destfd, filefd);
      if (red_dup2 < 0) {
            perror("Error: dup2()");
            return -1;
      }
      else return destfd;
}
