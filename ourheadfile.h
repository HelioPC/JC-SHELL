#ifndef __OHF__
    #define __OHF__
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <ctype.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <pthread.h>
   
    #define TRUE 1
    #define EXIT 0
    #define PATHNAME 1
    #define INT_CLEAR 64
    #define THREAD_CREATE_FAILED 12
    #define W_TOTAL_TIME 59
    #define W_NUM_ITER 31
    #define STR_CLEAR "clear"
    #define CLEAR() system(STR_CLEAR)
    #define OUTPUT_TXT "log.txt"
    #define READ_AND_APPEND "a+"
    #define READ_AND_WRITE "w+"

    #ifndef __COLORS__
        #define __COLORS__
        #define NORM "\033[m"
        #define GREEN "\033[32m"
        #define BLUE "\033[36m"
        #define D_BLUE "\033[34m"
        #define PURPLE "\033[35m"
        #define RED "\033[31m"
        #define GREY "\033[37m"
    #endif
    
    /* this TDA represents all the information needed to write to the file. */
    typedef struct{
        unsigned int iternum;
        time_t totaltime;
    } REGARQ;

    /* evaluates the command and returns its corresponding integer. */
    int command(char *);

    /* responsible thread to handle all the process deadline */
    void* monitor_Thread();

    /* returns the number of lines in file */
    int filelines(FILE *);

    /* returns 1 if the global file has the necessary information,
    otherwise returns 0. If they are valid, they are assigned to the global
    data structure. */
    int fileload();

    /* write all necessary information to the global file */
    void filewrite(pid_t, time_t);

    /* return 1 if the string is a number else 0 */
    int isnum(char *);
#endif
