#include "libhdcr.h"
#include "dip.h"

int __numberForegroundPixelNeighbors(IMAGE *img, int r, int c, uint8_t CFGval)
{
    int sum = 0;
    for (int i = r-1; i <= r+1; i++) {
        for (int j = c -1; j <= c+1; j++) {
            if (i != r || j != c) {
                if (img->raw_bits[i][j] == CFGval) sum++;
            }
        }
    }
    return sum;
}

/* the number of transitions from white to black in the sequence of
 * eight neighbors around the origin pixel
 */
int __BWTransitions(IMAGE *img, int r, int c, uint8_t CFGval)
{
    uint8_t N  = img->raw_bits[r-1][c];
    uint8_t NE = img->raw_bits[r-1][c+1];
    uint8_t NW = img->raw_bits[r-1][c-1];
    uint8_t E  = img->raw_bits[r][c+1];
    uint8_t S  = img->raw_bits[r+1][c];
    uint8_t SE  = img->raw_bits[r+1][c+1];
    uint8_t SW  = img->raw_bits[r+1][c-1];
    uint8_t W  = img->raw_bits[r][c-1];

    return ((N  != CFGval  && NE == CFGval)
           +(NE != CFGval  && E  == CFGval)
           +(E  != CFGval  && SE == CFGval)
           +(SE != CFGval  && S  == CFGval)
           +(S  != CFGval  && SW == CFGval)
           +(SW != CFGval  && W  == CFGval)
           +(W  != CFGval  && NW == CFGval)
           +(NW != CFGval  && N  == CFGval));
}

/* Pass 1
 * return true i.f.f
 *    The pixel is black and has eight neighbours
 *    2 <= B(i,j) <= 6
 *    A(i,j) = 1
 *    At least one of the north, east, and south neighbors is white
 *    At least one of east, south, and west neighbors is white
 */
int __ZSTest1(IMAGE *img, int r, int c, uint8_t CFGval)
{
    int numNeighbors = __numberForegroundPixelNeighbors(img, r, c, CFGval);

    uint8_t N = img->raw_bits[r-1][c];
    uint8_t E = img->raw_bits[r][c+1];
    uint8_t S = img->raw_bits[r+1][c];
    uint8_t W = img->raw_bits[r][c-1];

    return ((numNeighbors >=2 && numNeighbors<=6)) 
            && (__BWTransitions(img, r, c, CFGval)==1)
            && (N==CFGval || E==CFGval || S==CFGval)
            && (E==CFGval || S==CFGval || W==CFGval);
}

/*
 * Pass 2
 * return true i.f.f
 *     The pixel is black and has eight neighbours
 *     2 <= B(i,j) <= 6
 *     A(i,j) = 1
 *     At least one of the north, east, and west neighbors is white
 *     At least one of north, south, and west neighbors is white
 */
int __ZSTest2(IMAGE *img, int r, int c, uint8_t CFGval)
{
    int numNeighbors = __numberForegroundPixelNeighbors(img, r, c, CFGval);

    uint8_t N = img->raw_bits[r-1][c];
    uint8_t E = img->raw_bits[r][c+1];
    uint8_t S = img->raw_bits[r+1][c];
    uint8_t W = img->raw_bits[r][c-1];

    int bwTransitions = __BWTransitions(img, r, c, CFGval);

    int result = ((numNeighbors >=2 && numNeighbors<=6)
              && (bwTransitions==1)
              && (N==CFGval || E==CFGval || W==CFGval)
              && (N==CFGval || S==CFGval || W==CFGval));

    return result;
}
/* Zhang-Suen Thinning Algorithm
 *  CFGval = value of forgeound component either 0 or 255
 * 
 * note: only works on thresholded images
 */
void ZhangSuenThinning(IMAGE *img, uint8_t CFGval)
{
    int r, c;

    // create and zero out markers array
    uint8_t **markers = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(uint8_t));

    for (r=0; r<img->n_rows; r++)
        for (c=0; c<img->n_cols;c++)
            markers[r][c] = 0;

    //first pass
    for (r=1; r<img->n_rows-1; r++) {
        for (c=1; c<img->n_cols-1; c++) {
            if (img->raw_bits[r][c] != CFGval) continue;

            if (__ZSTest1(img, r, c, CFGval)==1) {
                markers[r][c] = 1;
            }
        }
    }
    //second pass
    for (r=1; r<img->n_rows-1; r++) {
        for (c=1; c<img->n_cols-1; c++) {                
            if (img->raw_bits[r][c] != CFGval) continue;

            if (__ZSTest2(img, r, c, CFGval)==1) {
                markers[r][c] = 1;
            }
        }
    }

    for(r=0; r<img->n_rows; r++) {
        for(c=0; c<img->n_cols; c++) {
            if (markers[r][c] == 1) {
                if (CFGval==255)
                    img->raw_bits[r][c] = 0;
                else
                    img->raw_bits[r][c] = 255;
            }
        }
    }
}
