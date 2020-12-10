#include <math.h>
#include "libhdcr.h"

double AverageImage(IMAGE *x)
{
    double totalPixels = x->n_rows*x->n_cols;

    double numForeground = 0;
    for (int r=0; r<x->n_rows; r++) {
        for (int c=0; c<x->n_cols; c++){
            if (x->raw_bits[r][c] == 255) totalPixels++;
        }
    }
    return totalPixels/numForeground;
}

double varianceImage(IMAGE *x)
{
    return 0;
}

double covarianceImages(IMAGE *x, IMAGE *y)
{
    return 0;
}

double SSIM(IMAGE *x, IMAGE *y)
{
    double ux = AverageImage(x);
    double uy = AverageImage(y);
    double sigmax = varianceImage(x);
    double sigmay = varianceImage(y);
    double cov = covarianceImages(x,y);

    double L = pow(2,8)-2;

    double c1 = pow(0.01*L,2);
    double c2 = pow(0.03*L,2);

    double SSIM = ((2*ux*uy+c1)*(2*cov+c2)/((pow(ux,2)+pow(uy,2)+c1)*(pow(sigmax,2) + pow(sigmay,2) + c2)));

    return SSIM;
}