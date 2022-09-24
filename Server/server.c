#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int establishConnection(int portNumber, int sd);

int main(int argc, char *argv[]){
    int sd; /*socket descriptor*/
    int flag; 
    int connected_sd; /*socket descriptor*/
    int rc; /*Return code*/
    struct sockaddr_in server_address;
    struct sockaddr_in from_address;
    char buffer[100];
    char output[100];
    //added
    char temp[100];
    socklen_t fromLength; 
    int portNumber; 
    int bytesRead ;
    FILE *outputFile;
    

    if (argc < 2){
        printf("Usage is: server <portNumber>\n"); 
        exit(1);
    }


    portNumber = atoi(argv[1]);
    sd = socket(AF_INET, SOCK_STREAM,0);

    
    fromLength = sizeof(struct sockaddr_in);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portNumber);
    server_address.sin_addr.s_addr = INADDR_ANY;

    
    rc = bind(sd, (struct sockaddr*)&server_address, sizeof(server_address));
    if (rc < 0){
        printf("bind error\n");
    }

    listen(sd, 5);
    connected_sd = accept(sd, (struct sockaddr*) &from_address, &fromLength);

    while(connected_sd){
        memset(buffer,0,100);
        //READ STATEMENT
        int sizeOfFileName;
        rc = read(connected_sd, &sizeOfFileName,sizeof(int));
        printf("read %d bytes to get the filename size\n",rc);
        printf("size of the file name prior to converson is %d bytes \n",sizeOfFileName);
        sizeOfFileName = ntohs(sizeOfFileName);
        printf("size of the file name after converson is %d bytes \n",sizeOfFileName);


        //READ STATEMENT
        int fileSize;
        rc = read(connected_sd, &fileSize, sizeof(int));
        printf("read %d bytes to get the filesize\n",rc);
        //printf("size of the file name prior to converson is %d bytes \n",fileSize);
        fileSize = ntohl(fileSize);
        printf("Size of file: %d bytes\n",fileSize);

        //READ STATEMENT
        rc = read(connected_sd, output, sizeOfFileName);
        //prints name of the file
        output[sizeOfFileName] = '\0';
        printf("Name of file: %s\n",output);

        int isDone = -1;
        isDone = strcmp(output, "Done");
        if (isDone == 0) {
            printf("Done ~kya");
            return 0;
        };    

        //READ FILECONTENTS YAY!
        if((outputFile = fopen(output, "wb")) == NULL){
            printf("open %s failed\n",output);
        }

        bytesRead = 0;
        while(bytesRead < fileSize){
            bytesRead+=1;
            char bufferLocal[1];
            rc = read(connected_sd, bufferLocal, 1);
            if (rc < 0){
                printf("LMAO bruh you are kinda sus omegalul! %d", rc);
                flag = 1;
                break;
            }
            rc = fwrite(bufferLocal, 1, 1, outputFile);
        }
        //WRITE STATEMNENT, ACK
        rc = write(connected_sd, &bytesRead, sizeof(int));

        fclose(outputFile);
    }

    return 0;
}
