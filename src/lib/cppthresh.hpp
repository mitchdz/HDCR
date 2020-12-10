#ifndef CPP_THRESH_H
#define CPP_THRESH_H
#ifdef __cplusplus
extern "C" {
#endif

#include "libhdcr.h"
void adaptiveThresholdOtsu(IMAGE *img, uint8_t *t);

#ifdef __cplusplus
}
#endif
 
#endif // CPP_THRESH_H