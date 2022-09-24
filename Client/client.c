#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int readfile(FILE *inputFile, FILE *outputFile);

int requestInfo();

int main(int argc, char *argv[]){
    int sd;
    struct sockaddr_in server_address;
    char filename[20];
    char buffer[100] = "hello world";
    int portNumber;
    char serverIP[29];
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

    printf("What is the name of the file you'd like to send");
    scanf("%s", filename);
    printf("filename to send it '%s'\n", filename);
    int sizeOfFileName = strlen(filename);
    int converted_sizeOfFileName = ntohs(sizeOfFileName);

    rc = write(sd, &converted_sizeOfFileName, sizeof(converted_sizeOfFileName));
    printf("wrote %d bytes to send the filesize\n", rc);
    if (rc < 0) {
        perror("error writing");
    }

    rc = write(sd, filename, converted_sizeOfFileName);
    printf("sent %d bytes\n to send the filename\n", rc);    
    if(rc < 0){
        perror("error writing");
    }
    return 0;
    //requestInfo();
}

int requestInfo()
{
    while(1){
        char iFile[20];
        char oFile[20];
        int isDone = -1;
        int firstIteration = 0; 
        FILE *inputFile, *outputFile;

        printf("%s","Please enter in an input file name.\n");
        scanf(" %[^\n]s", iFile);

        isDone = strcmp(iFile, "Done");
        if (isDone == 0) {
            printf("Done ~kya");
            return 0;
        }


        printf("%s","Please enter in an output file name.\n");
        scanf(" %[^\n]s",oFile);  
        int rc;
        char inputFileName[strlen(iFile)];
        char outputFileName[strlen(oFile)];


        memcpy(inputFileName, iFile, strlen(iFile));
        memcpy(outputFileName, oFile, strlen(oFile));
        printf("the input file name is %s, and the output file name is %s\n", inputFileName, outputFileName);

        if((inputFile = fopen(iFile, "rb")) == NULL) {
            printf("cannot open input file %s\n", inputFileName);
            exit(1);
        }
        if((outputFile = fopen(outputFileName, "wb"))== NULL){
            printf("cannot open output file %s\n", outputFileName);
            exit(1);
        }
        rc = readfile(inputFile,outputFile);
        rc = fclose(inputFile);
        if(rc<0){
            perror("close");
        }
        rc = fclose(outputFile);
        if(rc<0){
            perror("close");
        }
    }
    
    return 0;
}

#define BUFFSIZE 1000

int readfile(FILE *inputFile, FILE *outputFile)
{
    unsigned char buffer [BUFFSIZE];
    int numberOfBytes;
    int rc;
    int totalBytesRead = 0, totalBytesWritten = 0;

    numberOfBytes = fread(buffer,1, BUFFSIZE, inputFile);

    while(numberOfBytes > 0){
        totalBytesRead += numberOfBytes;
        rc = fwrite(buffer,1,numberOfBytes, outputFile);
        if(numberOfBytes != rc){
            perror("writing to file");
            exit(1);
        }
        totalBytesWritten += rc;
        numberOfBytes = fread(buffer,1,BUFFSIZE,inputFile);
    }
    printf("read %d bytes, and wrote %d bytes\n", totalBytesRead, totalBytesWritten);
    return 0;
}


