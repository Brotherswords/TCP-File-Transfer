#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int sd; /*socket descriptor*/
    int flag; 
    int connected_sd; /*socket descriptor*/
    int rc; /*Return code*/
    struct sockaddr_in server_address;
    struct sockaddr_in from_address;
    char output[100];
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

    //perform a bind on the socket and port number
    rc = bind(sd, (struct sockaddr*)&server_address, sizeof(server_address));
    if (rc < 0){
        printf("bind error\n");
    }
    while(1){
        //waiting for a client to connect
        listen(sd, 5);
        printf("Listening...\n");
        //indicates that someone has attempted to connect to the server
        connected_sd = accept(sd, (struct sockaddr*) &from_address, &fromLength);
        printf("LETS GO - Connection Found!\n");

        //while there is a connection to the server, we will perform the following actions
        while(connected_sd){
            //READ STATEMENT
            //Intake the size of the file name
            int sizeOfFileName;
            rc = read(connected_sd, &sizeOfFileName,sizeof(int));
            printf("read %d bytes to get the filename size\n",rc);
            printf("size of the file name prior to converson is %d bytes \n",sizeOfFileName);
            //convert it back to normal size from network order
            sizeOfFileName = ntohs(sizeOfFileName);
            printf("size of the file name after converson is %d bytes \n",sizeOfFileName);

    
            //READ STATEMENT
            //intake the size of the file itself
            int fileSize;
            rc = read(connected_sd, &fileSize, sizeof(int));
            printf("read %d bytes to get the filesize\n",rc);
            //printf("size of the file name prior to converson is %d bytes \n",fileSize);
            fileSize = ntohl(fileSize);
            printf("Size of file: %d bytes\n",fileSize);

            //READ STATEMENT
            //Intake the name of the file
            rc = read(connected_sd, output, sizeOfFileName);
            //prints name of the file
            //Add a null terminator to make sure no jargon is present at the end of the filename
            output[sizeOfFileName] = '\0';
            printf("Name of file: %s\n",output);

            //Checks if DONE has been entered and if so, breaks out of the loop, and goes back to listening.
            int isDone = -1;
            isDone = strcmp(output, "DONE");
            if (isDone == 0) {
                printf("Connection Closed :)\n");
                break;
            };    

            //READ FILECONTENTS YAY!
            //Opens an output file with the right name
            if((outputFile = fopen(output, "wb")) == NULL){
                printf("open %s failed\n",output);
            }

            bytesRead = 0;
            //READ STATEMENT
            //Reads the same number of bytes as the file size to ensure that no more or less bytes than neccesary are read
            while(bytesRead < fileSize){
                bytesRead+=1;
                char bufferLocal[1];
                //stores the character received from the input file into the bufferLocal
                rc = read(connected_sd, bufferLocal, 1);
                if (rc < 0){
                    printf("LMAO bruh why isnt it working! %d", rc);
                    flag = 1;
                    break;
                }
                //Then writes that one character into the outputFile.
                rc = fwrite(bufferLocal, 1, 1, outputFile);
            }
            //WRITE STATEMNENT, ACK
            //Sends to the server the number of bytes read to indicate that everything was read and received successfully.
            rc = write(connected_sd, &bytesRead, sizeof(int));

            fclose(outputFile);
        }
    }

    return 0;
}
