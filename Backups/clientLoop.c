#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readfile(FILE *inputFile, FILE *outputFile);

int requestInfo();

int main(int argc, char *argv[]){

    requestInfo();
}

int requestInfo()
{
    char iFile[20];
    char oFile[20];
    int isDone = -1;
    int firstIteration = 0; 
    FILE *inputFile, *outputFile;

    printf("%s","Please enter in an input file name.\n");
    scanf("%[^\n]s", iFile);
    printf("%s","Please enter in an output file name.\n");
    scanf(" %[^\n]s",oFile);  
    int rc;
    char inputFileName[strlen(iFile)];
    char outputFileName[strlen(oFile)];


    isDone = strcmp(iFile, "Done");
    if (isDone == 0) {
        printf("Done ~kya");
        return 0;
    }

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


