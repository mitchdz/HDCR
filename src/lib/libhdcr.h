#ifndef ECE576A_H
#define ECE576A_H

#include <stdio.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct IMAGE {
    int n_rows;
    int n_cols;
    uint8_t threshold;
    uint8_t **raw_bits;
} IMAGE;












#ifdef __cplusplus
}
#endif


#endif // ECE576A_H
