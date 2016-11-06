#ifndef SIG_HANDLER_H
#define SIG_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void ctrl_c_handler(int sig)
{
  
}


typedef void (*sighandler_t)(int);

/* This function  installs a signal handler for 'signo' signal */
sighandler_t signal(int signo, sighandler_t handler)
{
	struct sigaction act, oact;

	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	if( signo == SIGALRM ) {
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;  /* SunOS 4.x */
#endif
	} else {
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;  /* SVR4, 4.4BSD */
#endif
	}

	if( sigaction(signo, &act, &oact) < 0 )
		return (SIG_ERR);

	return(oact.sa_handler);
}

#endif