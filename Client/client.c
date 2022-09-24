#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>



int getFileLength(FILE* fp);

int main(int argc, char *argv[]){
    int sd;
    struct sockaddr_in server_address;
    char filename[100];
    int portNumber;
    char serverIP[29];
    FILE *inputFile;
    int rc = 0;

    if (argc < 3) {
        printf("Usage: %s <server ip> <port>\n", argv[0]);
        exit(1);
    }

    sd = socket(AF_INET, SOCK_STREAM, 0);

    portNumber = strtol(argv[2], NULL,10);
    strcpy(serverIP, argv[1]);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portNumber);
    server_address.sin_addr.s_addr = inet_addr(serverIP);

    if (connect(sd, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in)) < 0) {
        close(sd);
        perror("error connecting stream socket");
        exit(1);
    }


    while(1){
        memset(filename,0,100);
        printf("What is the name of the file you'd like to send?\n");
        scanf("%s", filename);
        char inputFileName[strlen(filename)];
        memcpy(inputFileName, filename, strlen(filename));
        inputFileName[strlen(filename)] = '\0';
        printf("filename to send it '%s'\n", inputFileName);
        printf("filename size to send it '%lu'\n", strlen(inputFileName));
        int sizeOfFileName = strlen(inputFileName);
        int converted_sizeOfFileName = ntohs(sizeOfFileName);
        //added

        //WRITE STATEMENT
        //Getting ready to send the size of the file name in bytes
        rc = write(sd, &converted_sizeOfFileName, sizeof(converted_sizeOfFileName));
        printf("wrote %d bytes to send the filename size\n", rc);
        if (rc < 0) {
            perror("error writing");
        }


        int isDone = -1;
        isDone = strcmp(inputFileName, "Done");
        if (isDone == 0) {
            int bytesToWrite = 0;
            int convertedBytesToWrite = htonl(bytesToWrite);
            rc = write(sd, &convertedBytesToWrite, sizeof(convertedBytesToWrite));
            rc = write(sd, &inputFileName, sizeof(inputFileName));
            printf("Done ~kya");
            return 0;
        };    
        //WRITE STATEMENT
        //Getting ready to send the size of the file in bytes
        if((inputFile = fopen(filename, "rb")) == NULL) {
            printf("cannot open input file %s\n", filename);
        }

        int bytesToWrite = getFileLength(inputFile);
        int convertedBytesToWrite = htonl(bytesToWrite);
        rc = write(sd, &convertedBytesToWrite, sizeof(convertedBytesToWrite));
        printf("wrote %d bytes to send the file length\n", bytesToWrite);
        //printf("converted %d bytes to send the file length\n", convertedBytesToWrite);
        //printf("sent %d bytes of the file size\n", rc);    
        if(rc < 0){ 
            perror("error writing");
        }

        //WRITE STATEMENT
        //Getting ready to send the file name in bytes
        rc = write(sd, &inputFileName, sizeof(inputFileName));
        if(rc < 0){ 
            perror("error writing");
        }
        //printf("sent %d bytes to send the filename\n", rc)


        //WRITE STATEMENT
        //Send contents of the file to the server
        //readfile(inputFile, sd);
        int totalBytesRead = 0;
        char c ;
        while (1)
        {
            c = fgetc (inputFile); // reading the file
            if (c == EOF){
                break;
            }
            rc = write(sd, &c, 1);
            totalBytesRead += 1;
        }
        printf("Closing the file test.c\n") ;
        fclose (inputFile ) ;
        //READ STATEMENT ACK from Server
        int totalBytesReceived = 0;
        rc = read(sd, &totalBytesReceived, sizeof(int));
        printf("ACK from server: Server Received %d bytes\n", totalBytesReceived);
    }

    return 0;
}

int getFileLength(FILE* fp){
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}



