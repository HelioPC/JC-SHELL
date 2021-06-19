#ifndef __TERM_H__
    #define __TERM_H__

    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <signal.h>

    /* this TDA represents all the information needed to recieve Terminals id. */
    typedef struct regter{
        unsigned int id_terminal;
        struct regter *next;
    } REGTER;

    /* this TDA represents our linked list of 'REGTER' */
    typedef struct{
        REGTER *first;
    } LISTTERMS;

    /* responsible thread to handle all the process terminal */
    void* regTerminalThread();

    /**/
    int exitterminal(char *);

    /**/
    int exitall(int);

    /**/
    int insert_new_terminal_id(LISTTERMS *, unsigned int);

    /**/
    LISTTERMS *listterm_init();

    /**/
    int remove_terminal_id(LISTTERMS *, unsigned int);

    /**/
    void freelsterm(LISTTERMS *);

    /**/
    int isinlistterm(LISTTERMS *, unsigned int);

    /**/
    int listtermtam(LISTTERMS *);

#endif
