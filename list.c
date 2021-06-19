#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

LIST_PROC* process_new(){
   LIST_PROC *ls;
   ls = (LIST_PROC *) malloc(sizeof(LIST_PROC));

   if(ls != NULL) ls->first = NULL;

   return ls;
}

void process_destroy(LIST_PROC *ls){
	PROCESS *proc, *nextproc;

	proc = ls->first;
	while (proc != NULL){
		nextproc = proc->next;
		free(proc);
		proc = nextproc;
	}
	free(ls);
}

void insert_new_process(LIST_PROC *ls, pid_t pid, time_t start_time){
	PROCESS *proc;

	proc = (PROCESS *) malloc (sizeof(PROCESS));
	proc->pid = pid;
	proc->start_time = start_time;
	proc->end_time = (time_t) -1;
	proc->next = ls->first;
	ls->first = proc;
}

void update_terminated_process(LIST_PROC *ls, pid_t pid, int status,
time_t end_time){
	PROCESS *proc = ls->first;

	while(proc != NULL && proc->pid != pid) proc = proc->next;
	
	if(proc == NULL) return;
	
	proc->status = status;
	proc->end_time = end_time;
}

void process_print(LIST_PROC *ls){
	PROCESS *proc;

	printf("\nProcess list:\n");
	proc = ls->first;
	while (proc != NULL){
		printf("%spid%s=%d%s\t", PURPLE, GREY, (int) proc->pid, NORM);
		printf("\t%sstatus%s=%d%s", PURPLE, GREY, proc->status, NORM);
		printf("\t%sduration%s=%lds%s\n", PURPLE, GREY,
		(long int) (proc->end_time - proc->start_time), NORM);
		proc = proc->next;
	}
	printf("\n---- end of list. ----\n");
}

time_t gettime(LIST_PROC *ls, pid_t pid){
	PROCESS *proc = ls->first;

	while(proc != NULL){
		if(proc->pid == pid) break;

		proc = proc->next;
	}

	if(proc == NULL) return (time_t) -PIDNOTFOUND;

	return (proc->end_time - proc->start_time);
}

void unfinished(LIST_PROC *ls, int fd){
	int buffersize = 80;
	char buffer[buffersize];
	PROCESS *proc = ls->first;

	while(proc != NULL){
		if(proc->end_time == (time_t) -1){
			sprintf(buffer, "pid=%d\n", proc->pid);
			write(fd, buffer, strlen(buffer));
		}

		proc = proc->next;
	}
}
