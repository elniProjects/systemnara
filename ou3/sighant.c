/*
* File:                 sighant.c
* Authur:               Elias Niko
* Email:                c18eno@cs.umu.se
* Current version:      1.0
* Date:                 2019-10-11
*/


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sighant.h"

typedef	void	Sigfunc(int);
extern int *process;


/*
* Reliable version of signal(), using POSIX sigaction().
* This function is coipied from a lesson resurce  



*/
Sigfunc *mysignal(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}

void sighandle(int a){
	(void)a;
	int i = 0;
	while(process[i]){
	      kill(process[i], SIGINT);
	      i++;
	}

}
