#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "hdcr.h"

void printhdcrHelp()
{
    printf("USAGE: \n hdcr -i inputfile.png -o outputfile.png [opt args]\n");
    printf("the following args can be enabled:\n");
    printf("\t-i <filename>\n");
    printf("\t\t<filename> is the name of the input file\n");
    printf("\t-o <filename>\n");
    printf("\t\t<filename> is the name of the output file\n");
    printf("\t-m <value>\n");
    printf("\t\t<value> MaxOutputValue should be either 0 or 255 where 0 ");
    printf("would indicate ncomponents, default 0\n");
    printf("\t-c <value>\n");
    printf("\t\t<value> is either 0 or 255 indicating ComponentGrayLevel, default 0\n");
    printf("\t-a <value>\n");
    printf("\t\tadaptive threshold type. Currently support 'otsu' and 'kittler' (default otsu)\n");
    printf("\t-v\n");
    printf("\t\tverbose (enabled by default)\n");
    printf("\t-s\n");
    printf("\t\tsilent, supress commandline output\n");
    printf("\t-D\n");
    printf("\t\tdebug, print to console and write images to filesystem (sets -w -v)\n");
    printf("\t-t\n");
    printf("\t\t(optional) threshold, value between 0 and 255\n");
    printf("\t-v\n");
    printf("\t\tverbose\n");
    printf("\t-b\n");
    printf("\t\tbenchmark, forego the actual project, and just benchmark library functions\n");
    printf("\t-h\n");
    printf("\t\tprint help menu\n");
}

int main(int argc,char* argv[]) {
    error_hdcr_t err = E_hdcr_GENERIC_ERROR;
    int MaxOutputValue = 0;
    int ComponentGrayLevel = 0; // 0 means black
    bool verbose = true;
    bool write = false;
    char inputFile[100];
    char outputFile[100];
    uint8_t threshold = 0;
    ADAPTIVE_THRESHOLD_TYPE att = ADAPTIVE_THRESHOLD_OTSU;
    bool attdo = true;
    bool benchmark = false;

    char*ifile;
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "ho:i:c:m:st:a:wDb")) != -1)
         switch (c)
         {
            case 'h':
              printhdcrHelp();
              return 1;
            case 'b':
              benchmark = true;
              break;
            case 'a':
              if ( !strcmp((const char*)"otsu", optarg) ){
                att = ADAPTIVE_THRESHOLD_OTSU;
              }
              else if ( !strcmp((const char*)"kittler", optarg) ) {
                att = ADAPTIVE_THRESHOLD_KITTLER;
              }
              else {
                att = ADAPTIVE_THRESHOLD_OTSU;
              }
              break;
            case 't':
              threshold = atoi(optarg);
              break;
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
             case 's':
              verbose = false;
              break;
            case 'c':
              ComponentGrayLevel = atoi(optarg);
              break;
            case 'w':
              write=true;
              break;
            case 'D':
              verbose=true;
              write=true;
              break;
            default:
              abort ();
         }

    if (inputFile == NULL)
        printError(err, "no input file specified");

    if (outputFile == NULL)
        printError(err, "no output file specified");

    // adaptive threshold if no user defined threshold
    attdo = (threshold == 0) ? true : false;

    err = hdcr_run_program(inputFile, outputFile, 
        attdo, 
        att, 
        threshold, 
        MaxOutputValue, 
        ComponentGrayLevel, 
        verbose,
        write,
        (char *) "png",
        benchmark);

    return 0;
}
