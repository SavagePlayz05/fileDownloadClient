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
    fprintf(s,"HELO");
    fgets(output, 100, s);
    printf("%s", output);
    
}