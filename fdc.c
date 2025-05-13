#include <stdio.h>
#include <libsocket/libinetsocket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

void getFileName(char *input, char *fileName){
    //find char seperating statements
    char *start= strchr(input, ' ');
    //increment once to get to the start of the fileName
    start++;
    strncpy(fileName, start, strlen(start));
    fileName[strcspn(fileName, "\n")] = 0;
}
int getSize(FILE *s, char fileName[100]){
        char output[100];
        int fileSize;
            //send SIZE to the server
            fprintf(s,"SIZE %s\n",fileName);
            //receive and parse the actual size in KB
            fgets(output, 100, s);
            printf("%s\n", output);
            sscanf(output, "+OK %d", &fileSize);
            printf("%d", fileSize);
            return fileSize;
        
}
void makeList(FILE *s, char input[100]){
    char output[100];
    //send LIST to the server
    fprintf(s,"%s",input);
            //get rid of OK output
            fgets(output, 100, s);
            //while there is still lines left, and the last line doesn't have . as element 0.
            while(fgets(output, 100, s) && (output[0] !='.')){
                printf("%s", output);
            }
}
void downloadFile(FILE *s, char *input, char *fileName, int fileSize) {
    FILE *file = fopen(fileName, "wb");
    char output[100];
    char buffer[fileSize];
    //send GET command
    fprintf(s, "%s", input);
    //get rid of +OK line
    fgets(buffer, sizeof(buffer), s);
    //read from server into buffer
    fread(buffer, 1, fileSize, s);

        //check if it's a text file
        if (strcmp(fileName + (strlen(fileName)) - 4, ".txt") == 0){
            printf("This is a Text File! Type D to Download or O to Open:\n");
            fgets(output, 2, stdin);
            //if they want it opened, print it out then break out of function, otherwise continue and download.
            if (output[0] =='O'){
                printf("%s", buffer);
                return;
            }         
        }

    //write from butter to local file
    fwrite(buffer, 1, fileSize, file);
    // close file and send complete message.
    fclose(file);
    printf("Successfully Downloaded %s\n", fileName);
}

void getAll(FILE *s, char *input, char *fileName, int fileSize){
    char output[100];
    int i=0;
    char temp[100][100];
    char fileNames[100][100];
    // call list
    fprintf(s,"LIST\n");
            fgets(output, 100, s);
            //read line by line, and copy each line into an array
            while(fgets(output, 100, s) && (output[0] !='.')){
                strcpy(temp[i], output);
                i++;
            }

            for (int j = 0; j<i; j++){
            //get the fileName out of the temp arr
            getFileName(temp[j],fileName);

            //setup input so it can be fowarded into downloadFile
            strcpy(input, "GET ");
            strcat(input, fileName);
                //download the file!
                downloadFile(s, input, fileName, fileSize);
                printf("\n");
                // and repeat!
            }
}
int main(int argc, char *argv[]){
    //call vars and set arrays
    char output[100];
    char input[100];
    char fileName[100];
    int fileSize =0;
    int quit = 0;
    char temp[100];
    char serverHash[100];
    //connect to server
    printf("connecting to: richmond.cs.sierracollege.edu\n");
    int fd = create_inet_stream_socket("london.cs.sierracollege.edu",
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
        printf("=====================MENU=====================\n[LIST] List All files available for Download \n[SIZE (FILE NAME)] Retrieves the size of a file \n[GET (FILE NAME)] Downloads a file from the server\n[GET ALL] Downloads ALL files stored on the server\n[PING] Checks that the server's still working (gives back +OK message)\n[QUIT] Close program");
        printf("What would you like to do?:");
        //get input
        fgets(input,100,stdin);
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
            makeList(s,input);
        }
        
        //print the size of a specified file
        if (strncmp(input, "SIZE ", 5) == 0) {
            getFileName(input, fileName);
            fileSize = getSize(s, fileName);
            printf("The File is %dKB\n", fileSize);
        }
        //catch GET inputs, but not GET ALL.
        if (strncmp(input, "GET ", 4) == 0 && strncmp(input, "GET ALL", 7) != 0) {
            // get file name
            getFileName(input, fileName);
            //Open new file to write to later. 
            FILE *file = fopen(fileName, "wb");

            //recopy str so we have the \n char to send to server

            
            //send SIZE command to get size of line in bytes for fwrite and fread
            fileSize = getSize(s, fileName);
            char buffer[fileSize];

            downloadFile(s, input, fileName, fileSize);


        }
        //implement ping command. very simple
        if (strcmp(input, "PING\n") ==0) {
            fprintf(s,"%s",input);
            fgets(output, 100, s);
            printf("%s\n", output);
        }
        if(strncmp(input, "GET ALL", 7) == 0){
            getAll(s,input, fileName, fileSize);
        }
}
}