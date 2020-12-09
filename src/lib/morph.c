#include <stdbool.h>
#include "libhdcr.h"
#include "thinning.h"
#include "dip.h"


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

    uint8_t **markers = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(uint8_t));
    for (r=0; r<img->n_rows; r++)
        for (c=0; c<img->n_cols;c++)
            markers[r][c] = img->raw_bits[r][c];

    for (r=1; r<img->n_rows-1; r++) {
        for(c=1; c<img->n_cols-1; c++) {
            if (markers[r][c] == CFGval) {
                img->raw_bits[r-1][c] = CFGval;
                img->raw_bits[r-1][c+1] = CFGval;
                img->raw_bits[r-1][c-1] = CFGval;
                img->raw_bits[r][c-1] = CFGval;
                img->raw_bits[r][c+1] = CFGval;
                img->raw_bits[r+1][c] = CFGval;
                img->raw_bits[r+1][c+1] = CFGval;
                img->raw_bits[r+1][c-1] = CFGval;
            }
        }
    }

}

/* erosion with 3x3 kernel:
 *          1 1 1
 *          1 1 1
 *          1 1 1
 */
erodeImage3by3Kernel(IMAGE *img, uint8_t CFGval)
{
    int r,c;

    // copy image 
    uint8_t **markers = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(uint8_t));
    for (r=0; r<img->n_rows; r++)
        for (c=0; c<img->n_cols;c++)
            markers[r][c] = img->raw_bits[r][c];

    for (r=1; r<img->n_rows-1; r++) {
        for(c=1; c<img->n_cols-1; c++) {
            if (markers[r-1][c]   == CFGval
             && markers[r-1][c+1] == CFGval
             && markers[r-1][c-1] == CFGval
             && markers[r][c-1]   == CFGval
             && markers[r][c+1]   == CFGval
             && markers[r+1][c]   == CFGval
             && markers[r+1][c+1] == CFGval)
            {
                continue;
            }
            else {
                if (CFGval==255) {
                    img->raw_bits[r][c] = 0;
                }
                else {
                    img->raw_bits[r][c] = 255;
                }
            }
        }
    }
}

/*
 * opening is the dilation of the erosion of a set A by a structuring element B:
 * A ∘ B = ( A ⊖ B ) ⊕ B
 * 
 */
void openImage(IMAGE *img, uint8_t CFGval)
{
    erodeImage3by3Kernel(img, CFGval);
    dilateImage3by3Kernel(img, CFGval);
}


void subtractImage(IMAGE *img, IMAGE *tmp)
{


}

/* skeletonization of an image
 *
 * skeletonization can be achieved by using dilation and erosion.
 * 
 * using a 3x3 cross kernel as the structuring element as such:
 *     0 1 0
 *     1 1 1
 *     0 1 0
 * 
 * Algorithm is as follows:
 *      Sn(X) = (X ⊖ nB) - (X ⊖ nB)∘B
 * where
 *      B is the structuring element
 *      X is the binary image
 * 
 * 
 */
void skeletizeImage(IMAGE *img, uint8_t CFGval)
{


    // copy the image

    //IMAGE temp;
    //uint8_t **tempRawBits = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(uint8_t));
    //temp->raw_bits = tempRawBits;
    //for (int r=0; r<img->n_rows; r++)
        //for (int c=0; c<img->n_cols;c++)
            //temp->raw_bits[r][c] = img->raw_bits[r][c];
    //temp->n_cols = img->n_cols;
    //temp->n_rows = img->n_rows;


    ZhangSuenThinning(img, CFGval);
    //while (1) {
    //    ZhangSuenThinning(img, CFGval);

    //    // open image
    //    openImage(img, CFGval);

    //    // subtract open from original image
    //    temp = subtractImage(img, temp));

    //    // erode image
    //}

    //matfree(temp.raw_bits);
}


void removeBranchPoints(IMAGE *img, uint8_t CFGval)
{
    


}