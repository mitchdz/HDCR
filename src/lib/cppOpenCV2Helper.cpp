#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include <bitset>
#include "cppthresh.hpp"
#include "cppmorph.hpp"
#include "thresh.h"
#include "libhdcr.h"

cv::Mat convertIMAGEtoMat(IMAGE *img)
{
    cv::Mat temp = cv::Mat(img->n_rows, img->n_cols, CV_8U);
    for (int r=0; r<img->n_rows; r++) {
        for (int c=0; c<img->n_cols; c++) {
            temp.row(r).col(c) = img->raw_bits[r][c];
        }
    }
    return temp;
}

void convertMatToIMAGE(cv::Mat mat, IMAGE *img)
{
    for (int r=0; r<img->n_rows; r++) {
        for (int c=0; c<img->n_cols; c++) {
            img->raw_bits[r][c] = (uint8_t)(*mat.row(r).col(c).data);
        }
    }
}
