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
    #define THREAD_CREATE_FAILED 12
    #define STR_CLEAR "clear"
    #define CLEAR() system(STR_CLEAR)
    #define OUTPUT_TXT "log.txt"
    #define READ_AND_WRITE "a+"

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

    /* evaluates the command and returns its corresponding integer. */
    int command(char *);

    /* responsible thread to handle all the process deadline */
    void* monitor_Thread();
#endif
