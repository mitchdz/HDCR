#include "libhdcr.h"
#include "thinning.h"


/* inverts image's pixels */
void invertImage(IMAGE *img)
{
    for (int r = 0; r < img->n_rows;r++) {
        for (int c =0; c < img->n_cols;c++) {
            if (img->raw_bits[r][c] == 0)
                img->raw_bits[r][c] = 255;
            else
                img->raw_bits[r][c] = 0;
        }
    }
}

/* performed as the thinning of the negated image */
void __thickenImage(IMAGE *img, uint8_t CFGval) 
{
    invertImage(img);
    ZhangSuenThinning(img, CFGval);
    invertImage(img);
}

/* Performs the thickening operation on an image n times
 *
 * thickening adds pixels to the exterior of objects.
 * 
 */
void thickenImage(IMAGE *img, int n, uint8_t CFGval)
{
    for (int i = 0; i < n; i++) {
        __thickenImage(img, CFGval);
    }
}

/* removes spurious pixels. For example:
 *   0 0 0
 *   0 1 0
 *   0 0 0 
 *  the one will be removed.
 */
//void cleanSpuriousPixels(IMAGE *img)
//{
//    for (int r = 1; r < img->n_rows-1; r++)
//        for (int c = 1; c < img->n_cols-1; c++) 
//            if (numberZeroPixelNeighbors(img, r, c) == 0)
//                img->raw_bits[r][c] = 0;
//}

void dilateImage3by3Kernel(IMAGE *img, uint8_t CFGval)
{
    int r,c;
    //uint8_t kernel[3][3] = {{1,1,1},{1,1,1},{1,1,1}};

    uint8_t temp;
    //dummy img
    uint8_t **dummy = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(uint8_t));
    for (r=0; r<img->n_rows; r++) {
        for(c=0; c<img->n_cols; c++) {
            temp = img->raw_bits[r][c];
            dummy[r][c] = temp;
        }
    }

    for (r=0; r<img->n_rows; r++) {
        for(c=0; c<img->n_cols; c++) {
            if (dummy[r][c] == CFGval) {

            }
        }
    }



    // TODO: implement
}

void removeBranchPoints(IMAGE *img)
{
    //TODO: implement
}