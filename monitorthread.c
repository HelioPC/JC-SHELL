#include <sys/wait.h>
#include <time.h>
#include "headers/list.h"
#define __FILE_FUNCS__
#include "headers/ourheadfile.h"
#include <fcntl.h>
#include <sys/stat.h>

#define __USE_POSIX 1

#include <signal.h>


extern int exit1;
extern int numChildren;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond_var;
extern pthread_cond_t cond_var_2;
extern LIST_PROC *list;


void *monitor_Thread(void *args){
	int status, await;

	if(args != NULL){
		printf("%sThis thread doesn't need any args. ", RED);
		printf("Unexpected behavior is expected%s", NORM);
	}

	/* this is the main loop */
	while(1){

		/*waits the process in execution to terminate*/
		pthread_mutex_lock(&mutex);
		if(numChildren > 0){ /* Critical section */
			pthread_mutex_unlock(&mutex);
			await = wait(&status);
			
			pthread_mutex_lock(&mutex); /* Critical section */
			update_terminated_process(list, await, WEXITSTATUS(status),
			time(NULL));
			numChildren--; /* End of critical section */
			pthread_mutex_unlock(&mutex);
			
			filewrite(await, gettime(list, await));
			pthread_cond_signal(&cond_var_2);
			continue;
		}
		pthread_mutex_unlock(&mutex);

		/* terminates the thread when there is no more task to wait for */
		pthread_mutex_lock(&mutex);
		if(exit1 == 1 && (numChildren) == 0){ /* Critical section*/
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&mutex);

		/*sleep for one second then continuos the cicle */
		pthread_mutex_lock(&mutex);
		while(exit1 != 1 && (numChildren) == 0)/* Critical section */
			pthread_cond_wait(&cond_var, &mutex);
		pthread_mutex_unlock(&mutex);
	}
}
