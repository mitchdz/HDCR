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
#include "thinning.h"

void run_benchmarks(IMAGE *img, char* imageName);

char *concat3Strings(char* str1, char* str2, char* str3)
{
    char * qq = (char*) malloc((strlen(str1)+strlen(str2)+strlen(str3))*sizeof(char));
    strcpy(qq, str1);
    strcat(qq, str2);
    strcat(qq, str3);

    return qq;
}


void __skeletonize(IMAGE *img, uint8_t CGL, bool verbose, bool write)
{
    if (verbose) printf("Skeletonizing Image...\n");
    skeletizeImage(img, CGL);
    if (verbose) printf("\tDone\n");
    if (write) writePNG(img, (char *)"demo/Circuit2-1_skeletonize.png");
}


void __removeBranchpoints(IMAGE *img, uint8_t CGL, bool verbose, bool write)
{
    if (verbose) printf("Removing branchpoints...\n");
    removeBranchPoints(img, CGL);
    if (verbose) printf("\tDone\n");
    if (write) writePNG(img, (char *)"demo/Circuit2-1_branchpoint.png");
}

error_hdcr_t __threshold(IMAGE *img, uint8_t threshold, bool verbose, bool write)
{
    if (verbose) printf("Thresholding...\n");
    threshold2DPseudoArray(img->raw_bits, img->n_rows, img->n_cols, threshold);
    if (write) writePNG(img, (char *)"demo/Circuit2-1_thresh.png");
    if (verbose) printf("\tDone.\n");
    return E_hdcr_SUCCESS;
}


error_hdcr_t __thicken(IMAGE *img, uint8_t CGL, bool verbose, bool write)
{
    error_hdcr_t err = E_hdcr_SUCCESS;

    int numThickening = 3;
    if (verbose) printf("Thickening %d time(s)...\n", numThickening);
    err = thickenImage(img, numThickening, CGL);
    if (verbose) printf("\tDone\n");
    if (write) writePNG(img, (char *)"demo/Circuit2-1_thicken.png");

    return err;
}

error_hdcr_t __dilate(IMAGE* img, uint8_t CGL, bool verbose, bool write)
{
    error_hdcr_t err = E_hdcr_SUCCESS;

    if (verbose) printf("dilating with a 3x3 kernel...\n");
    err = dilateImage3by3Kernel(img, CGL);
    if (verbose) printf("\tDone\n");
    if (write) writePNG(img, (char *)"demo/Circuit2-1_Dilation.png");

    return err;
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
    bool write,
    char* imgType,
    bool benchmark
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
    
    /* Connected Component Image */
    IMAGE CCImage;
    uint8_t **CCImage_raw_bits = 
        matalloc(img.n_rows, img.n_cols, 0, 0, sizeof(uint8_t));

    CCImage.n_cols = img.n_cols;
    CCImage.n_rows = img.n_rows;
    CCImage.raw_bits = CCImage_raw_bits;

    // if benchmark flag is enabled, run benchmarks
    if (benchmark) {
        run_benchmarks(&img, inputImageFileName);
        goto cleanup;
    }

    /* find adaptive threshold */
    if (verbose) {
        printf("%s is a %dx%d grayscale image\n", inputImageFileName, img.n_rows, img.n_cols);
    }

    if (adaptiveThreshold) {
        if (verbose) printf("Using Adaptive Threshold %s...\n", getATTType(att));
        adaptiveThresholdWithMethod(&img, att, &threshold);
        if (verbose) printf("\tDone.\n\tDetermined Threshold: %d\n", threshold);
    }
    else {
        threshold = inputThreshold;
    }


    /******** threshold **********/
    err = __threshold(&img, threshold, verbose, write);
    if (err != E_hdcr_SUCCESS) {
        printError(err, (char *)"error thresholding"); goto cleanup;
    }

    /******** thicken **********/
    err = __thicken(&img, CGL, verbose, write);
    if (err != E_hdcr_SUCCESS) {
        printError(err, (char *)"error thickening"); goto cleanup;
    }

    /******** dilate **********/
    err = __dilate(&img, CGL, verbose, write);
    if (err != E_hdcr_SUCCESS) {
        printError(err, (char *)"error dilating"); goto cleanup;
    }

    // thin the image consecutively
    for (int i=0; i<5; i++)
        ZhangSuenThinning(&img, CGL);

    // erode the image consecutively
    for (int i=0; i<4; i++)
        erodeImage3by3Kernel(&img, CGL);

    if (write) writePNG(&img, (char *)"demo/Circuit2-1_skeletonize.png");



    int numberOfComponents;
    iterativeCCL(&img, CCImage.raw_bits, CGL, &numberOfComponents, verbose);
    if (verbose) printf("there are %d sets of components\n", numberOfComponents);


    // if any group has less than 





    // usually you would wnat to skeletonize to detect branchpoints, but
    // in our case it's actually better to not. This is because if you fully
    // skeletize the image, some branches will be connected in the 8-connected
    // sense.
    /******** skeletonize **********/
    //__skeletonize(&img, CGL, verbose, write);

    /********* remove branchpoints ********/
    //__removeBranchpoints(&img, CGL, verbose, write);
   

    /********* Find centroids & bounding box ********/
    //if (verbose) printf("Finding Centroids and bounding box...\n");
    // TODO: implement
    
    /********* Merge Centroid Clusters ********/
    // TODO: implement

    /********* SSIM to classify image ********/
    // TODO: implement

    /********* merge cluster bounding boxes with branchpoints & connect *******/
    // TODO: implement

cleanup:
    matfree(CCImage_raw_bits);
    matfree(img.raw_bits);
    return err;
} // end of hdcr_run_program

void run_benchmarks(IMAGE *img, char* imageName)
{
    printf("Timing benchmarks for libhdcr using %s\n", imageName);

    /* erosion */





}