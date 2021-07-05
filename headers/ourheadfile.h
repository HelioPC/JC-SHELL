/*
*All essential functions and constants used by the JCShell project
*(header file).
*
*Developed by group nº02
*
*ELIÚDE PATRÍCIO DE CARVALHO VEMBA - (ID Github) HelioPC
*LUDMILO HUEBA CAMBAMBI - (ID Github) Ludmilo-cambambi
*PEDRO MANUEL DOMINGOS - (ID Github) pedro7-7-7
*LUCÍLIO TÉRCIO GOMES - (ID Github) luciliogomez
*/

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
    #include "color.h"
    #include "commands.h"

    #define VERSION "V.1.5.3"
    #define VERSION_TERM "V.2.5"
   
    #ifdef __CMDS__
        #define CLEAR() system(STR_CLEAR)
    #endif

    #ifdef __CONST__
        #define TRUE 1
        #define BUFFERSIZE 100
        #define VECTORSIZE 7
        #define FILENAMESIZE 27
    #endif

    #ifdef __FILE_PIPE__
        #define OUTPUT_TXT "log.txt"
        #define READ_AND_APPEND "a+"
        #define READ_AND_WRITE "w+"
        #define TMP_DIR "/tmp/.jcshell"
        #define NAMED_PIPE "jcshell-in"
        #define NAMED_PIPE_AUX "/tmp/.jcshell/stats-"
        #define NAMED_PIPE_REG "/tmp/.jcshell/reg-in"
    #endif
    
    /* this TDA represents all the information needed to write to the file. */
    typedef struct{
        unsigned int iternum;
        time_t totaltime;
    } REGARQ;

    /* evaluates the command and returns its corresponding integer. */
    int command(char *);

    #ifdef __MNT_THREAD__
        /* responsible thread to handle all the process deadline */
        void *monitor_Thread();
    #endif

    #ifdef __FILE_FUNCS__
        /* returns the number of lines in file */
        int filelines(FILE *);

        /* returns 1 if the global file has the necessary information,
        otherwise returns 0. If they are valid, they are assigned to the global
        data structure. */
        int fileload();

        /* write all necessary information to the global file */
        void filewrite(pid_t, time_t);
    #endif

    /* return 1 if the string is a number else 0 */
    int isnum(char *);

    /* erase the string str */
    int strdel(char *);

    /* Writes the number of unfinished processes and the total execution time
    of all processes in the file descriptor.
    On sucess returns 1, otherwise returns 0.*/
    int stats(int, int, int);

    /* lists all files and directories present in the program directory. */
    void ls();

#endif
