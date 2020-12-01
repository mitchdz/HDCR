#ifndef MCC_H
#define MCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libhdcr.h"
#include <stdint.h>
#include <stdbool.h>

void OverlayComponentsOntoImage(IMAGE *img, uint8_t **componentMatrix, int nc,
    bool CGL, bool MOV);
void iterativeCCL(IMAGE *img, uint8_t **outccM, bool CGL, int *nc, bool verbose);

#ifdef __cplusplus
}
#endif

#endif /*MCC_H*/