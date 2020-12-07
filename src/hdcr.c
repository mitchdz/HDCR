#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "hdcr.h"
#include "thresh.h"
#include "IO.h"
#include "morph.h"

char *concat3Strings(char* str1, char* str2, char* str3)
{
    char * qq = (char*) malloc((strlen(str1)+strlen(str2)+strlen(str3))*sizeof(char));
    strcpy(qq, str1);
    strcat(qq, str2);
    strcat(qq, str3);

    return qq;
}

void printWelcomeMessage(char* input, char* output)
{
    printf("Hand Drawn Circuit Recognizer\n");
    printf("\t input: %s\n", input);
    printf("\t output: %s\n", output);
}

/*
 * hdcr steps:
 * 1) threshold image
 * 2) find center of each component
 * 3) determine what each component is
 * 4) follow the wires to the next component and connect
 * 5) print the linked list of connected components
 */
error_hdcr_t hdcr_run_program(
    char* inputImageFileName,
    char* outputImageFileName,
    bool adaptiveThreshold,
    ADAPTIVE_THRESHOLD_TYPE att,
    uint8_t inputThreshold,
    uint8_t MOV, //MaxOutputValue
    uint8_t CGL, // ComponentGrayLevel
    bool verbose,
    char* imgType
    )
{
    //TODO: error detection
    //TODO: check that att is a valid enum

    if (verbose) printWelcomeMessage(inputImageFileName, outputImageFileName);

    error_hdcr_t err = E_hdcr_SUCCESS;
    uint8_t threshold = 0;

    /*** read the input file and store it into IMAGE struct ***/

    IMAGE img; 
    readPNGandClose(inputImageFileName, &img);
    
    // if verbose, print image size
    if (verbose) {
        printf("%s is a %dx%d grayscale image\n", inputImageFileName, img.n_rows, img.n_cols);
    }

    if (adaptiveThreshold) {
        if (verbose) printf("Using Adaptive Threshold %s...\n", getATTType(att));
        adaptiveThresholdWithMethod(&img, att, &threshold);
        if (verbose) printf("Done. Determined Threshold: %d\n", threshold);
    }
    else {
        threshold = inputThreshold;
    }

    /*** threshold the image ***/

    if (verbose) printf("Thresholding %s with a value of %d...\n", inputImageFileName, threshold);
    threshold2DPseudoArray(img.raw_bits, img.n_rows, img.n_cols, threshold);
    if (verbose) printf("Done.\n");

    char *outname = concat3Strings(outputImageFileName, "_thresh.", imgType);
    if (verbose) printf("Printing thresholded image to %s ...\n", outname);
    writePNG(img.raw_bits, (char*)outname, img.n_rows, img.n_cols);
    if (verbose) printf("Done.\n");


    /******** thicken **********/
    int numThickening = 3;
    if (verbose) printf("Thickening %d times...\n", numThickening);
    thickenImage(&img, numThickening);
    if (verbose) printf("Done\n");

    /******** dilate **********/
    if (verbose) printf("dilating with a 3x3 kernel...\n");
    dilateImage3by3Kernel(&img);
    if (verbose) printf("Done\n");

    /********* remove branchpoints ********/
    if (verbose) printf("Removing branchpoints...\n");
    removeBranchPoints(&img);
    if (verbose) printf("Done\n");

    /********* Find centroids & bounding box ********/
    if (verbose) printf("Finding Centroids and bounding box...\n");
    // TODO: implement
    
    /********* Merge Centroid Clusters ********/
    // TODO: implement

    /********* SSIM to classify image ********/
    // TODO: implement

    /********* merge cluster bounding boxes with branchpoints & connect *******/
    // TODO: implement


    return err;
}



error_hdcr_t  detectNumberOfCircuitComponents(IMAGE *img)
{
    return E_hdcr_NOT_IMPLEMENTED;
}
