#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){
    char s1[20];
    char *s2 = "exit";

    fgets(s1, 20, stdin);

    if(strcmp(s1, "exit\n") == 0){
        sprintf(s1, "exit-%d", getpid());
    }

    if(strstr(s1, s2) != NULL){
        printf("%s\n", strlen(s1) > 6 ? s1 : s2);
    }
    
    printf("c = %ld\n ", strlen(s1));
    printf("pid = %s", (s1+5));

    return 0;
}
