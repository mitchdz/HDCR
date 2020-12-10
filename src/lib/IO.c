#include "IO.h"
#include "libhdcr.h"

/* @brief writePNG writes a png to a file
 * @param raw_data uint8_t pseudo 2-D array created by matalloc
 * @param n_rows number of rows of the pseudo 2D array
 * @param n_cols number of columns of the pseudo 2D array
 */
void writePNG(IMAGE *img, char* filename)
{
    PNGFILE *output_png = pngOpen(filename, (char *)"w");
    pngWriteHdr(output_png, img->n_rows, img->n_cols);
    for (int r = 0; r < img->n_rows; r++) 
        pngWriteRow(output_png, img->raw_bits[r]);
    free(output_png);
}

void readPNGandClose(char *input_file, IMAGE *img)
{   
    int nRows, nCols;
    PNGFILE *pngfile = pngOpen(input_file, (char *)"r");
    pngReadHdr(pngfile, &nRows, &nCols);

    img->n_rows = nRows;
    img->n_cols = nCols;

    uint8_t **raw_bits = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(uint8_t));

    img->raw_bits = raw_bits;

    // store entire image into memory
    for (int r = 0; r < img->n_rows; r++) pngReadRow(pngfile, img->raw_bits[r]);

    pngClose(pngfile);
}