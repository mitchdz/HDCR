#ifndef IO_H
#define IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "dip.h"
#include "pngio.h"
#include "libhdcr.h"

/* @brief writePNG writes a png to a file
 */
void writePNG(IMAGE *img, char* filename);

void readPNGandClose(char *input_file, IMAGE *img);

#ifdef __cplusplus
}
#endif

#endif //IO_H