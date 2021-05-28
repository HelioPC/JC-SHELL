#ifndef __OHF__
    #define __OHF__
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <sys/types.h>
    #include <unistd.h>
     #include <pthread.h>
   
    #define TRUE 1
    #define EXIT 0
    #define PATHNAME 1
    #define INT_CLEAR 64
    #define STR_CLEAR "clear"
    #define NORM "\033[m"
    #define GREEN "\033[32m"
    #define BLUE "\033[36m"
    #define D_BLUE "\033[34m"
    #define PURPLE "\033[35m"
    #define RED "\033[31m"
    #define GREY "\033[37m"
    #define CLEAR() system(STR_CLEAR)
    
    
    /* Type to represent a process */
    typedef struct process{
        int pid;    /* The process id */
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
    void insert_new_process(LIST_PROC *, int, time_t);

    /* update_teminated_process - updates endtime of element with pid 'pid' */
    void update_terminated_process(LIST_PROC *, int, int, time_t);

    /* process_print - print the content of list 'list' to standard output */
    void process_print(LIST_PROC *);

    /* evaluates the command and returns its corresponding integer. */
    int command(char *);

    /* responsible thread to handle all the process deadline */
    void* monitor_Thread();
#endif
