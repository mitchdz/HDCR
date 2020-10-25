#ifndef THRESH_H
#define THRESH_H

#define _USE_MATH_DEFINES //this is for vscode
#include <math.h>

#include <stdint.h>
#include <stdio.h>
// include all of the libdip header and source files
// note: this looks awful. Instead we could just include
// the .h and .c files and provide path to gcc with -I../libdip/src
// but alas I am here not doing that.
#include "../libdip/src/pngio.h"
#include "../libdip/src/matfree.c"
#include "../libdip/src/pngReadRow.c"
#include "../libdip/src/padarray.c"
#include "../libdip/src/pngErrorHandler.c"
#include "../libdip/src/pngReadHdr.c"

#include "IO.h"

typedef enum {
    UINT8_T,
    INT32_T,
} TYPE;

/* hw4 error codes for debugging the program */
enum _hdcr_error
{
    E_HDCR_SUCCESS,
    E_HDCR_FILE_NOT_FOUND,
    E_HDCR_GENERIC_ERROR,
    E_HDCR_ARRAY_SIZE_MISMATCH,
    E_HDCR_NOT_IMPLEMENTED,
    E_HDCR_RECURSIVE_FIRST_LAST_NOT_FOUND,
};

typedef enum _hdcr_error error_t;

/* helper function to easily decode error message */
struct _errordesc{
    int code;
    char *message;
} errordesc[] = {
    { E_HDCR_SUCCESS,  (char *)"No error" },
    { E_HDCR_FILE_NOT_FOUND, (char *)"File not found" },
    { E_HDCR_GENERIC_ERROR,  (char *)"Generic Error" },
    { E_HDCR_ARRAY_SIZE_MISMATCH,  (char *)"Array sizes do not match" },
    { E_HDCR_NOT_IMPLEMENTED,  (char *)"Not implemented yet" },
    { E_HDCR_RECURSIVE_FIRST_LAST_NOT_FOUND, (char *)"Recursive first and last index not able to be found"},
};

void printError(error_t E, char *msg)
{
    printf("Error %d: %s %s\n", errordesc[E].code, errordesc[E].message ,msg);
}

void printHelp()
{
    printf("TODO: print help\n");
}

/* @brief zeros a psuedo 2D array created by matalloc
 *
 * @param array pseudo 2D array
 * @param n_rows number of rows of the array
 * @param n_cols number of columns of the array
 * @param type what type the array is using the enum TYPE
 *
 * @see matalloc
 * @return error_t
 */
error_t zeroPsuedo2DArray(void** array, int32_t n_rows, int32_t n_cols,
        TYPE type)
{
    for (int r = 0; r < n_rows; r++) {
        for (int c = 0; c < n_cols; c++) {
            switch(type) {
                case INT32_T:
                    ((int32_t**)array)[r][c] = 0;
                    break;
                case UINT8_T:
                    ((uint8_t **)array)[r][c] = 0;
                    break;
            }
        }
    }
    return E_HDCR_SUCCESS;
}

#endif // THRESH_H


