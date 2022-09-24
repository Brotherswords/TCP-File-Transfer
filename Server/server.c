#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int sd;
    int connected_sd;
    int rc;
    struct sockaddr_in server_address;
    struct sockaddr_in from_address;
    char buffer[100];
    int flags = 0;
    socklen_t fromLength; 
    int portNumber; 
    
    if (argc < 2){
        printf("Usage is: server <portNumber>\n"); 
        exit(1);
    }


    portNumber = atoi(argv[1]);
    sd = socket(AF_INET, SOCK_STREAM,0);

    printf("Got here!");
    printf("%d",sd);
    printf("%d",portNumber);
    
    
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
    memset(buffer,0,100);

    
    int sizeOfFileName;
    rc = read(connected_sd, &sizeOfFileName,sizeof(int));
    printf("read %d bytes to get the filesize\n",rc);
    printf("size of the file prior to converson is %d bytes \n",sizeOfFileName);
    sizeOfFileName = ntohs(sizeOfFileName);
    printf("size of the file after converson is %d bytes \n",sizeOfFileName);
    rc = read(connected_sd, &buffer, sizeOfFileName);
    if (rc <0 ){
        perror("read error");
        exit(1);
    }

    return 0;
}
