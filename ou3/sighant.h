/*
* File:                 sighant.h
* Authur:               Elias Niko
* Email:                c18eno@cs.umu.se
* Current version:      1.0
* Date:                 2019-10-11
*/




typedef void Sigfunc(int);
/* Reliable version of signal(), using POSIX sigaction().  */
Sigfunc *mysignal(int signo, Sigfunc *func);
/*
 * this function kills every process with SIGINT

 * Argument: a as a int.
 
 Return: -
*/
void sighandle(int a);
