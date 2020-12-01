#include "opencv2/highgui.hpp"
#include <limits.h>
#include <iostream>
#include <vector>
#include <bitset>
#include "cppthresh.hpp"
#include "thresh.h"

using namespace std;
using namespace cv;

//int otsu_method(float *histogram, long int total_pixels) {
void adaptiveThresholdOtsu(IMAGE *img, uint8_t *t)
{
    long int total_pixels = img->n_rows*img->n_cols;
    uint8_t raw_histogram[256];
    convert2DPseudoArrayToHistogram(img->raw_bits, img->n_rows,img->n_cols, raw_histogram);

    


    float histogram[256];
    for (int i = 0; i < 256; i++) {
        histogram[i] = (float)raw_histogram[i]/(float)total_pixels;
    }

    double probability[256], mean[256];
    double max_between, between[256];
    int threshold;

    for(int i = 0; i < 256; i++) {
        probability[i] = 0.0;
        mean[i] = 0.0;
        between[i] = 0.0;
    }

    probability[0] = histogram[0];
    for(int i = 1; i < 256; i++) {
        probability[i] = probability[i - 1] + histogram[i];
        mean[i] = mean[i - 1] + i * histogram[i];
    }

    threshold = 0;
    max_between = 0.0;

    for(int i = 0; i < 255; i++) {
        if(probability[i] != 0.0 && probability[i] != 1.0)
            between[i] = pow(mean[255] * probability[i] - mean[i], 2) / (probability[i] * (1.0 - probability[i]));
        else
            between[i] = 0.0;
        if(between[i] > max_between) {
            max_between = between[i];
            threshold = i;
        }
    }

    *t = threshold;

    return;
}

/*
void adaptiveThresholdOtsu(IMAGE *img, uint8_t *t)
{
    // convert grayscale image to histogram
    uint8_t histogram[256];
    convert2DPseudoArrayToHistogram(img->raw_bits, img->n_rows,img->n_cols, histogram);

    // get total number of pixels
    int total = round(img->n_rows * img->n_cols);

    // find histogram min,max vals
    uint8_t minval = __DBL_MAX__, maxval = __DBL_MIN__, minloc=0, maxloc=255;
    for (int i = 0; i < 256; i++) {
        if (histogram[i] < min) {
            min = histogram[i];
            minloc = i;
        }
        if (histogram[i] > max) {
            max = histogram[i];
            maxloc = i;
        }
    }

    vector<double> levels;

    //Mat inputMat(256, 1, CV_64F, histogram);
    Mat inputMat;

    float range[] = { 0, 256 };
    calcHist( img->raw_bits, 1, 0, Mat(), inputMat, 1, 256, uniform, accumulate)

    for (int i=minloc; i <maxloc; i++) {
        Mat mask;
        Mat invmask;
        inRange(inputMat, Scalar(0), Scalar(i), mask);
        bitwise_not(mask, invmask);
        vector<double> mean1, sdev1, mean2, sdev2;
        meanStdDev(inputMat,mean1,sdev1,mask);
        meanStdDev(inputMat, mean2, sdev2, invmask);

        int w1 = countNonZero(mask);
        int w2 = countNonZero(invmask);
        double w12 = (double)w1/total;
        double w22 = (double)w2/total;
        levels.push_back( w12*pow(sdev1[0],2) + w22*pow(sdev2[0],2) );
    }

    int minpos = 0;
    double currentmin = levels[0];
    for (long unsigned int i=1; i < levels.size(); i++) {
        if (levels[i] < currentmin) {
            currentmin = levels[i];
            minpos = i;
        }
    }

    *t = (int)minloc + minpos;
    return;
}
*/