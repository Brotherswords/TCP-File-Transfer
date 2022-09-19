#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readfile(FILE *inputFile, FILE *outputFile);

int main(int argc, char *argv[])
{

    
    FILE *inputFile, *outputFile;
    char inputFileName[20], outputFileName[20];
    int rc;

    if (argc< 3) {
        printf("Usage: inputFileName outputFileName\n");
        exit(1);
    }

    memcpy(inputFileName, argv[1], strlen(argv[1]));
    memcpy(outputFileName, argv[2], strlen(argv[2]));
    
    printf("the input file name is %s, and the output file name is %s\n", inputFileName, outputFileName);

    if((inputFile = fopen(argv[1], "rb")) == NULL) {
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


