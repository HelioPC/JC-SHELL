#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include "ourheadfile.h"

extern pthread_mutex_t mutex;
extern int Exit;
extern int numChildren;
extern LIST_PROC *list;
extern sem_t sem;
extern sem_t sem_sleep;

void* monitor_Thread(void *args){
	int status, await;

	if(args != NULL){
		printf("%sThis thread doesn't need any args. ", RED);
		printf("Unexpected behavior is expected%s", NORM);
	}

	/*this is the main loop*/
	while(1){
		pthread_mutex_lock(&mutex);
		if(numChildren > 0){
			pthread_mutex_unlock(&mutex);
			/*waits the process in execution to terminate*/
			await = wait(&status);
			pthread_mutex_lock(&mutex);
			update_terminated_process((LIST_PROC*)(list), await, status,
			time(NULL));
			(numChildren)--;
			pthread_mutex_unlock(&mutex);
			sem_post(&sem);
			continue;
		}
		pthread_mutex_unlock(&mutex);

		/*sleep for one second then continuos the cicle */
		pthread_mutex_lock(&mutex);
		if(Exit != 1){ /* Critical section*/
			pthread_mutex_unlock(&mutex);
			sem_wait(&sem_sleep);
			continue;
		}
		pthread_mutex_unlock(&mutex);
		
		pthread_mutex_lock(&mutex);
		if(Exit == 1 && (numChildren) == 0){
			/* terminates the thread when there is no more task to wait for*/
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&mutex);
	}
 }