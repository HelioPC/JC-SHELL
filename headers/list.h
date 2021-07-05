/*
*Linked list of processes interface used by the JCShell project (header file).
*
*Developed by group nº02
*
*ELIÚDE PATRÍCIO DE CARVALHO VEMBA - (ID Github) HelioPC
*LUDMILO HUEBA CAMBAMBI - (ID Github) Ludmilo-cambambi
*PEDRO MANUEL DOMINGOS - (ID Github) pedro7-7-7
*LUCÍLIO TÉRCIO GOMES - (ID Github) luciliogomez
*/

#ifndef __LIST_H__
    #define __LIST_H__
    #include <time.h>
    #include <sys/types.h>
    #include "color.h"

    #define PIDNOTFOUND 142

    /* Type to represent a process */
    typedef struct process{
        pid_t pid;    /* The process id */
        int status; /* The process termination status */
        time_t start_time;  /* Initial process time */
        time_t end_time;    /* Final process time */
        struct process *next;   /* Pointer to the next process */
    }PROCESS;

    /* Type to represent a linked list of processes */
    typedef struct{
        PROCESS *first; /* A pointer to the firt element on the list */
    }LIST_PROC;

    /* process_new - allocates memory for LIST_PROC and initializes it */
    LIST_PROC* process_new();

    /* process_destroy - free memory of LIST_PROC and all its items */
    void process_destroy(LIST_PROC *);

    /* insert_new_process - insert a new item with process id and its start
    time in list 'list' */
    void insert_new_process(LIST_PROC *, pid_t, time_t);

    /* update_teminated_process - updates endtime of element with pid 'pid' */
    void update_terminated_process(LIST_PROC *, pid_t, int, time_t);

    /* process_print - print the content of list 'list' to standard output */
    void process_print(LIST_PROC *);

    /* gettime - returns the execution time of a process */
    time_t gettime(LIST_PROC *, pid_t);

    /* returns the number of unfinished processes */
    int unfinished(LIST_PROC *);
#endif