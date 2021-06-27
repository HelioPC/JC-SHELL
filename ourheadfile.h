#ifndef __OHF__
    #define __OHF__
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <errno.h>
    #include <ctype.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <pthread.h>

    #define VERSION "V.1.5.3"
    #define VERSION_TERM "V.2.0"
   
    #define TRUE 1
    #define EXIT 2
    #define EXIT_GLOBAL 3
    #define PATHNAME 1
    #define INT_CLEAR 64
    #define STATS 41
    #define PID 73
    #define BUFFERSIZE 100
    #define VECTORSIZE 7
    #define FILENAMESIZE 27

    #ifndef __OUR_ERRNO__
        #define __OUR_ERRNO__
        #define THREAD_CREATE_FAILED 162 /* Failed to create a POSIX thread */
        #define FILE_CREATE_FAILED 155 /* Failed to create a file */
        #define OPEN_FILE_FAILED 171 /* Failed to open a file */
    #endif

    #define STR_CLEAR "clear"
    #define CMD_EXIT "exit"
    #define CMD_STATS "stats"
    #define CMD_EXIT_GLOBAL "exit-global"
    #define OUTPUT_TXT "log.txt"
    #define READ_AND_APPEND "a+"
    #define READ_AND_WRITE "w+"
    #define TMP_DIR "/tmp/.jcshell"
    #define NAMED_PIPE "jcshell-in"
    #define NAMED_PIPE_AUX "/tmp/.jcshell/stats-"
    #define NAMED_PIPE_REG "/tmp/.jcshell/reg-in"
    #define CLEAR() system(STR_CLEAR)

    #ifndef __COLORS__
        #define __COLORS__
        #define NORM "\033[m"
        #define GREEN "\033[32m"
        #define YELLOW "\033[33m"
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

    /* erase the string str */
    int strdel(char *);

    /* Writes the number of unfinished processes and the total execution time
    of all processes in the file descriptor.
    On sucess returns 1, otherwise returns 0.*/
    int stats(int, int, int);

#endif
