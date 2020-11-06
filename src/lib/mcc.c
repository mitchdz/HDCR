/* mcc (maximally connected components) */

#include <stdbool.h>
#include <stdlib.h>
#include "mcc.h"
#include "thresh.h"
#include <limits.h>

struct mapNode {
    int key;
    int val;
    struct mapNode *next;
};

static bool checkForeground(int value, bool CGL)
{
    bool foreground;
    if (CGL) { //ComponentGrayLevel is 1 meaning black=foreground
        foreground = ( value > 0) ? true : false;
    }
    else {
        foreground = ( value < 255) ? true : false;
    }
    return foreground;
}


void pushMapNode(struct mapNode **head, int key, int value)
{
    struct mapNode *new = (struct mapNode *)malloc(sizeof(struct mapNode));
    new->key = key;
    new->val = value;

    new->next = (*head);
    (*head) = new;
}

struct mapNode *findMapNode(struct mapNode *head, int key)
{
    while (head != NULL) {
        if (head->key == key) return head;
        head = head->next;
    }
    return NULL;
}


void iterativeCCL(IMAGE *img, uint8_t **outccM, bool CGL, int *nc, bool verbose)
{
    int r,c,i,M;
    int n[8], NW, N, NE, W, E, SW, S, SE; // neighbors
    int **l = matalloc(img->n_rows, img->n_cols, 0, 0, sizeof(int));
    for(r=0;r<img->n_rows;r++){
        for(c=0;c<img->n_cols;c++){
            l[r][c] = 0;
        }
    }

    int nextLabel = 1;
    for (r=1;r<img->n_rows-1;r++) { // raster scanning
        for (c=1;c<img->n_cols-1;c++) {
            // only worry about foreground pixels
            checkForeground(img->raw_bits[r][c], CGL) ? (l[r][c] = nextLabel++) : (l[r][c] = 0);
        }
    }
    bool change=false;
    do
    {
        change = false;
        for (r =1; r < img->n_rows-1;r++)
            for (c = 1; c < img->n_cols - 1; c++)
                if (l[r][c] != 0) {
                    NW = l[r-1][c-1];   N  = l[r-1][ c ];   NE = l[r-1][c+1];
                    W  = l[ r ][c-1];                       E  = l[ r ][c+1];
                    SW = l[r+1][c-1];   S  = l[r+1][ c ];   SE = l[r+1][c+1];

                    // get 8 neighbor pixels (8-connectivity)
                    n[0] = (NW > 0) ? NW : 0; n[1] = (N > 0) ? N : 0; n[2] = (NE > 0) ? NE : 0;
                    n[3] = (W > 0 ) ? W  : 0;                         n[4] = (E > 0)  ? E : 0;
                    n[5] = (SW > 0) ? SW : 0; n[6] = (S > 0) ? S : 0; n[7] = (SE > 0) ? SE : 0;

                    M = INT_MAX;
                    for (i = 0; i < 8; i++) {
                        if (M > n[i] && n[i] != 0)
                            M = n[i];
                    }

                    if (M != l[r][c]) {
                        l[r][c] = M;
                        change = true;
                    }
                }
        for (r = img->n_rows-1; r > 0; r--)
            for (c = img->n_cols-1; c > 0; c--)
                if (l[r][c] != 0) {
                    NW = l[r-1][c-1];   N  = l[r-1][ c ];   NE = l[r-1][c+1];
                    W  = l[ r ][c-1];                       E  = l[ r ][c+1];
                    SW = l[r+1][c-1];   S  = l[r+1][ c ];   SE = l[r+1][c+1];

                    // get 8 neighbor pixels (8-connectivity)
                    n[0] = (NW > 0) ? NW : 0; n[1] = (N > 0) ? N : 0; n[2] = (NE > 0) ? NE : 0;
                    n[3] = (W > 0 ) ? W  : 0;                         n[4] = (E > 0)  ? E : 0;
                    n[5] = (SW > 0) ? SW : 0; n[6] = (S > 0) ? S : 0; n[7] = (SE > 0) ? SE : 0;

                    M = INT_MAX;
                    for (i = 0; i < 8; i++) {
                        if (M > n[i] && n[i] != 0)
                            M = n[i];
                    }

                    if (M != l[r][c]) {
                        l[r][c] = M;
                        change = true;
                    }
                }
    } // end do
    while (change == true);

    /* the below code uses a linked list to create a key/value pair to map the setID to a smaller value. The reason for
     * this is because l is a pseudo 2D int array, whereas outCCM is a pseudo 2D uint8_t array. int->uint8_t causes an
     * overflow because a lot of the setID value in this sequential method exceed 255. There is a slim chance for an
     * edge case that the overflow perfectly matches another set. */
    struct mapNode *mnhead = (struct mapNode*)malloc(sizeof(struct mapNode));
    mnhead->key=0;
    mnhead->val=0;
    mnhead->next = NULL;
    struct mapNode *tmpmn = NULL;

    int key;
    int valcount = 1;
    // have to copy each value of l into outccM
    for (r = 1; r < img->n_rows-1; r++) { // raster scanning
        for (c = 1; c < img->n_cols-1; c++) {
            if (!checkForeground(img->raw_bits[r][c], CGL)) continue;
            key = l[r][c];
            tmpmn = findMapNode(mnhead, key);
            if (tmpmn == NULL) {
                pushMapNode(mnhead, key, valcount);
                outccM[r][c] = valcount;
                valcount++;
            }
            else {
                outccM[r][c] = tmpmn->val;
            }
        } // end col 2nd pass
    } // end row 2nd pass

    // dynamically allocate an array of the amount of components there are
    int *setCounts = (int *)malloc(sizeof(int)*valcount);
    for (i = 0; i < valcount;i++) setCounts[i] = 0;
    int numSets = 0;

    // determine how many pixels are in each component
    for (r = 1; r < img->n_rows-1; r++) {
        for (c = 1; c < img->n_cols - 1; c++) {
            setCounts[outccM[r][c]]++;
        }
    }

    // if verbose, print how many pixels in each component
    for (i=0;i<valcount;i++) {
        if (setCounts[i] != 0)
            if (verbose) printf("set %d\t%d\n",i,setCounts[i]);
    }

    // free dynamically allocated setCounts array
    free(setCounts);

    // set the numbercomponents value
    *nc = valcount - 1 ;
}

void OverlayComponentsOntoImage(IMAGE *img, uint8_t **ccM, int nc, bool CGL,
    bool MOV)
{
    int r,c, k, outputPixelValue, maxOutputValue;

    if (MOV) { maxOutputValue = 255; }
    else     { maxOutputValue = nc; }

    for (r = 1; r < img->n_rows-1; r++) { // raster scanning
        for (c = 1; c < img->n_cols-1; c++) {

             // only worry about foreground pixels
            if (!checkForeground(img->raw_bits[r][c], CGL)) continue;
 
            k = ccM[r][c];
            if (CGL) { outputPixelValue = round(1.0*k*maxOutputValue/nc);    }
            else     { outputPixelValue = round(1.0*(k-1)*maxOutputValue/nc);}

            img->raw_bits[r][c] = outputPixelValue;

        } // end col 2nd pass
    } // end row 2nd pass
    return;
}