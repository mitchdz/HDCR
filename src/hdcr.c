#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hdcr.h"
#include "thresh.h"
#include "mcc.h"
#include "IO.h"
#include "Hoshen-Kopelmanmcc.h"

error_ECE576A_t hdcr_run_program();

void printECE576AHW5Help()
{
    printf("USAGE\n");
    printf("the following flags can be enabled:\n");
    printf("\t-i <filename>\n");
    printf("\t\t<filename> is the name of the input file\n");
    printf("\t-o <filename>\n");
    printf("\t\t<filename> is the name of the output file\n");
    printf("\t-m <value>\n");
    printf("\t\t<value> MaxOutputValue should be either 0 or 255 where 0 ");
    printf("would indicate ncomponents\n");
    printf("\t-c <value>\n");
    printf("\t\t<value> is either 0 or 1 indicating ComponentGrayLevel\n");
    printf("\t-v\n");
    printf("\t\tverbose\n");
}

int main(int argc,char* argv[]) {
    error_ECE576A_t err = E_ECE576A_GENERIC_ERROR;
    int MaxOutputValue = 1;
    int ComponentGrayLevel = 1;
    bool verbose = false;
    char inputFile[100];
    char outputFile[100];

    char*ifile;
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "ho:i:c:m:v")) != -1)
         switch (c)
         {
            case 'h':
              printECE576AHW5Help();
              return 1;
            case 'm':
              MaxOutputValue = *optarg;
              break;
            case 'o':
              strcpy(outputFile, (const char *)optarg);
              break;
            case 'i':
              ifile = optarg;
              strcpy(inputFile, ifile);
              break;
             case 'v':
              verbose = true;
              break;
            case 'c':
              ComponentGrayLevel = atoi(optarg);
              break;
            default:
              abort ();
         }

    if (inputFile == NULL)
        printError(err, "no input file specified");

    if (inputFile == NULL)
        printError(err, "no input file specified");

    err = hdcr_run_program();

    return 0;
}

error_ECE576A_t hdcr_run_program()
{
    printf("TODO: hdcr_run_program");
    return E_ECE576A_GENERIC_ERROR;
}
