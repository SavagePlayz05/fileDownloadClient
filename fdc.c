#include <stdio.h>
#include <libsocket/libinetsocket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"
int main(int argc, char *argv[]){
    //call vars and set arrays
    char output[100];
    char input[100];
    char fileName[100];
    int quit = 0;

    //connect to server
    printf("connecting to: richmond.cs.sierracollege.edu\n");
    int fd = create_inet_stream_socket("richmond.cs.sierracollege.edu",
                            "3456", LIBSOCKET_IPv4, 0);
    //exit if you cannot connect
    if (fd < 0)
    {
        printf("Can't make connection\n");
        exit(1);
    }
    FILE *s = fdopen(fd, "r+");
    while(!quit){
        //prompt user for input
        printf("What would you like to do?:");
        fgets(input,100,stdin);
        printf("%s", input);
        fprintf(s, "%s",input);
        fflush(s);

        //chceck if user wants to end
        if (strcmp(input, "QUIT\n") == 0) {
            fprintf(s,"%s",input);

            fclose(s);
            close(fd);
            quit = 1;
        }

        if (strcmp(input, "LIST\n") ==0) {
            //while there is still lines left, and the last line doesn't have . as element 0.
            while(fgets(output, 100, s) && (output[0] !='.')){
                printf("%s", output);
            }
        }
        if (strncmp(input, "GET ", 4) == 0) {
            // remove +OK line
            for (int i =0; i< (strlen(input) - 4); i++){
                fileName[i] = input[i + 4];
            }
            printf("%s", fileName);
            fgets(output, 100, s);
            FILE *file = fopen(fileName, "w");


        }
    }
}