#ifndef MCC_H
#define MCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libhdcr.h"
#include <stdint.h>
#include <stdbool.h>

error_hdcr_t OverlayComponentsOntoImage(IMAGE *img, uint8_t **componentMatrix, int nc,
    bool CGL, bool MOV);
error_hdcr_t iterativeCCL(IMAGE *img, uint8_t **outccM, bool CGL, int *nc, bool verbose);
error_hdcr_t removeSmallComponents(IMAGE *img, uint8_t **ccM, int nc, uint8_t CGL, int *newNum);

#ifdef __cplusplus
}
#endif

#endif /*MCC_H*/