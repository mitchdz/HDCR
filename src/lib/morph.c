#include <stdbool.h>
#include "libhdcr.h"
#include "thinning.h"
#include "dip.h"


error_hdcr_t invertImage(IMAGE *img)
{
    // basic error checking
    if (img->n_rows == 0 
        || img->n_cols == 0
        || img->raw_bits == NULL) 
    {            
        return E_hdcr_STRUCT_PARAMETERS_NOT_SET;
    }

    for (int r = 0; r < img->n_rows;r++) {
        for (int c =0; c < img->n_cols;c++) {
            if (img->raw_bits[r][c] == 0)
                img->raw_bits[r][c] = 255;
            else
                img->raw_bits[r][c] = 0;
        }
    }
    return E_hdcr_SUCCESS;
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
error_hdcr_t thickenImage(IMAGE *img, int n, uint8_t CFGval)
{
    // basic error checking
    if (img->n_rows == 0 
        || img->n_cols == 0
        || img->raw_bits == NULL) 
    {            
        return E_hdcr_STRUCT_PARAMETERS_NOT_SET;
    }

    for (int i = 0; i < n; i++) {
        __thickenImage(img, CFGval);
    }
    return E_hdcr_SUCCESS;
}

/* removes spurious pixels. For example:
 *   0 0 0
 *   0 1 0
 *   0 0 0 
 *  the one will be removed.
 */
//error_hdcr_t cleanSpuriousPixels(IMAGE *img, uint8_t CFGval)
//{
//    // basic error checking
//    if (img->n_rows == 0 
//        || img->n_cols == 0
//        || img->raw_bits == NULL) 
//    {            
//        return E_hdcr_STRUCT_PARAMETERS_NOT_SET;
//    }
//
//    for (int r = 1; r < img->n_rows-1; r++)
//        for (int c = 1; c < img->n_cols-1; c++) 
//            if (numberZeroPixelNeighbors(img, r, c) == 0)
//                img->raw_bits[r][c] = 0;
//    return E_hdcr_SUCCESS;
//}



error_hdcr_t dilateImage3by3Kernel(IMAGE *img, uint8_t CFGval)
{
    // basic error checking
    if (img->n_rows == 0 
        || img->n_cols == 0
        || img->raw_bits == NULL) 
    {            
        return E_hdcr_STRUCT_PARAMETERS_NOT_SET;
    }

    int r,c;

    uint8_t **markers = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(uint8_t));
    for (r=0; r<img->n_rows; r++)
        for (c=0; c<img->n_cols;c++)
            markers[r][c] = img->raw_bits[r][c];

    // hardcoded true(3) kernel
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
    return E_hdcr_SUCCESS;
}







/* erosion with 3x3 kernel:
 *          1 1 1
 *          1 1 1
 *          1 1 1
 */
error_hdcr_t erodeImage3by3Kernel(IMAGE *img, uint8_t CFGval)
{
    // basic error checking
    if (img->n_rows == 0 
        || img->n_cols == 0
        || img->raw_bits == NULL) 
    {            
        return E_hdcr_STRUCT_PARAMETERS_NOT_SET;
    }
    
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
    return E_hdcr_SUCCESS;
}



/* opening is the dilation of the erosion of a set A by a structuring element B:
 * A ∘ B = ( A ⊖ B ) ⊕ B
 */
error_hdcr_t openImage(IMAGE *img, uint8_t CFGval)
{
    // basic error checking
    if (img->n_rows == 0 
        || img->n_cols == 0
        || img->raw_bits == NULL) 
    {            
        return E_hdcr_STRUCT_PARAMETERS_NOT_SET;
    }
 
    erodeImage3by3Kernel(img, CFGval);
    dilateImage3by3Kernel(img, CFGval);

    return E_hdcr_SUCCESS;
}

error_hdcr_t subractImage(IMAGE *x, IMAGE *y, uint8_t CFGval)
{
     for (int r=0; r<x->n_rows; r++) {
        for (int c=0; c<x->n_cols; c++) {
            if (y->raw_bits[r][c] == CFGval && x->raw_bits[r][c] == CFGval)
                if (CFGval == 255)
                    y->raw_bits[r][c] == 0;
                else
                    y->raw_bits[r][c] == 255;
                
        }
    }
    return E_hdcr_SUCCESS;
}

// y into x
error_hdcr_t bitwise_or(IMAGE *x, IMAGE *y, uint8_t CFGval)
{
    for (int r=0; r<x->n_rows; r++) {
        for (int c=0; c<x->n_cols; c++) {
            if (y->raw_bits[r][c] == CFGval) 
                x->raw_bits[r][c] == CFGval;
        }
    }
    return E_hdcr_SUCCESS;
}




error_hdcr_t skeletizeImage(IMAGE *img, uint8_t CFGval)
{
    /* skeletonization is simply just repeated erosion. */
    // example algo: http://opencvpython.blogspot.com/2012/05/skeletonization-using-opencv-python.html

    // //copy the image
    // IMAGE temp;
    // uint8_t **tempRawBits = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(uint8_t));
    // temp.raw_bits = tempRawBits;
    // for (int r=0; r<img->n_rows; r++)
    //     //for (int c=0; c<img->n_cols;c++)
    //         //temp->raw_bits[r][c] = img->raw_bits[r][c];
    // temp.n_cols = img->n_cols;
    // temp.n_rows = img->n_rows;

    // IMAGE skel;
    // uint8_t **skelRawBits = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(uint8_t));
    // skel.raw_bits = skelRawBits;
    // for (int r=0; r<img->n_rows; r++)
    //     for (int c=0; c<img->n_cols;c++)
    //         skel.raw_bits[r][c] = 0;
    // skel.n_cols = img->n_cols;
    // skel.n_rows = img->n_rows;

    for (int i =0; i<4; i++)
        erodeImage3by3Kernel(img, CFGval);

    //while (1) {
    //    // erode
    //    erodeImage3by3Kernel(img, CFGval);

    //    // dilate
    //    dilateImage3by3Kernel(temp, CFGval);

    //    // subtract
    //    subtractImage(img, temp, CFGval);

    //    // bitwise_or
    //    bitwise_or(skel, temp);

    //    if (!countNonZero(img)) break;
    //}

    // matfree(temp.raw_bits);
    return E_hdcr_SUCCESS;
}

error_hdcr_t removeBranchPoints(IMAGE *img, uint8_t CFGval)
{


   return E_hdcr_NOT_IMPLEMENTED;
}