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
    int fileSize;
    int quit = 0;
    char temp[100];

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

    //open filestream using server
    FILE *s = fdopen(fd, "r+");
    //get inital +OK Greeting Message
    fgets(output, 100, s);

    //start loop for actual program.
    while(!quit){
        //print menu and ask what to do
        printf("=====================MENU=====================\n[LIST] List All files available for Download \n[SIZE (FILE NAME)] Retrieves the size of a file \n[GET (FILE NAME)] Downloads a file from the server\n[PING] Checks that the server's still working (gives back +OK message)\n");
        printf("What would you like to do?:");
        //get input
        fgets(input,100,stdin);
        printf("%s", input);

        //chceck if user wants to end
        if (strcmp(input, "QUIT\n") == 0) {
            fprintf(s,"%s",input);
            fgets(output, 100, s);
            fclose(s);
            close(fd);
            quit = 1;
        }
        //print list if they type LIST
        if (strcmp(input, "LIST\n") ==0) {
            fprintf(s,"%s",input);
            fgets(output, 100, s);
            //while there is still lines left, and the last line doesn't have . as element 0.
            while(fgets(output, 100, s) && (output[0] !='.')){
                printf("%s", output);
            }
        }
        
        //print the size of a specified file
        if (strncmp(input, "SIZE ", 5) == 0) {
            fprintf(s,"%s",input);
            fgets(output, 100, s);
            printf("%s\n", output);
            sscanf(output, "+OK %d", &fileSize);
            printf("The File is %dKB\n", fileSize);
        }
        //catch GET inputs, but not GET ALL.
        if (strncmp(input, "GET ", 4) == 0 && strncmp(input, "GET ALL", 7) != 0) {
            // get file name
            strncpy(fileName, input + 4, sizeof(fileName));
            //strip newline so it doesn't bug out file name
            fileName[strcspn(fileName, "\n")] = 0;
            //Open new file to write to later. 
            FILE *file = fopen(fileName, "w");

            //recopy str so we have the \n char to send to server
            strncpy(fileName, input + 4, sizeof(fileName));
            
            //send SIZE command to get size of line in bytes for fwrite and fread
            fprintf(s, "SIZE %s", fileName);
            fgets(output, 100, s);
            //turn str into INT
            sscanf(output, "+OK %d", &fileSize);
            //create buffer
            char buffer[fileSize];
            //send original GET command
            fprintf(s, "%s", input);
            //get rid of +OK line
            fgets(output, 100, s);
            //read the data from the server, store in buffer
            fread(buffer, 1, fileSize, s);

            //write data from buffer to file on client
            fwrite(buffer, 1, fileSize, file);
            //close file
            fclose(file);
            //confirm it printed properly
            printf("Successfully Downloaded %s", fileName);

        }
        //implement ping command. very simple
        if (strcmp(input, "PING\n") ==0) {
            fprintf(s,"%s",input);
            fgets(output, 100, s);
            printf("%s\n", output);
        }
        if (strncmp(input, "HASH ", 5) == 0) {
            strncpy(fileName, input + 5, sizeof(fileName));
            //strip newline so it doesn't bug out file name
            fileName[strcspn(fileName, "\n")] = 0;
            fprintf(s, "SIZE %s", fileName);
            fgets(output, 100, s);
            //turn str into INT
            sscanf(output, "+OK %d", &fileSize);
            //create buffer
            char buffer[fileSize];
            
            
            //send HASH command
            fprintf(s, "%s", input);
            //get rid of +OK line
            fgets(output, 100, s);
            //read the data from the server, store in buffer
            fread(buffer, 1, fileSize, s);
            char *hashed = md5(buffer,fileSize);
            
            printf("%s", hashed);
        }
}
}