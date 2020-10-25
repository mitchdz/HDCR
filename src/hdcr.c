#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include "hdcr.h"

char *input_file = "input_image.png";
error_t hdcr();

// main function to parse commandline params
int main(int argc,char* argv[]) {
    error_t error = hdcr();
    return 0;
}

error_t hdcr()
{
    error_t error = E_HDCR_SUCCESS;
    int32_t r, n_cols, n_rows;
    PNGFILE *pngfile = pngOpen(input_file, "r");
    pngReadHdr(pngfile, &n_rows, &n_cols);

    // store entire image into memory
    uint8_t **grayscale = matalloc(n_rows, n_cols, 0, 0, sizeof(uint8_t));
    for (r = 0; r < n_rows; r++) pngReadRow(pngfile, grayscale[r]);
    return error;
}
