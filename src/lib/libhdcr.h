#ifndef ECE576A_H
#define ECE576A_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

typedef struct IMAGE {
    int n_rows;
    int n_cols;
    uint8_t threshold;
    uint8_t **raw_bits;
} IMAGE;


enum _hdcr_error
{
    E_hdcr_SUCCESS,
    E_hdcr_FILE_NOT_FOUND,
    E_hdcr_GENERIC_ERROR,
    E_hdcr_ARRAY_SIZE_MISMATCH,
    E_hdcr_NOT_IMPLEMENTED,
    E_hdcr_RECURSIVE_FIRST_LAST_NOT_FOUND
};
typedef enum _hdcr_error error_hdcr_t;

void printError(error_hdcr_t E, char *msg);

#ifdef __cplusplus
}
#endif

#endif // ECE576A_H