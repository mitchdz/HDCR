#ifndef MORPH_H
#define MORPH_H

//void cleanSpuriousPixels(IMAGE *img);
error_hdcr_t thickenImage(IMAGE *img, int n, uint8_t CFGval);
error_hdcr_t dilateImage3by3Kernel(IMAGE *img, uint8_t CFGval);
error_hdcr_t removeBranchPoints(IMAGE *img, uint8_t CFGval);

/* inverts image's pixels, must be thresholded to be 0 or 255*/
error_hdcr_t invertImage(IMAGE *img);

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
 */
error_hdcr_t skeletizeImage(IMAGE *img, uint8_t CFGval);

error_hdcr_t openImage(IMAGE *img, uint8_t CFGval);
error_hdcr_t erodeImage3by3Kernel(IMAGE *img, uint8_t CFGval);


#endif // MORPH_H