#ifndef HDCR_H
#define HDCR_H

#include <stdbool.h>
#include "libhdcr.h"
#include "mcc.h"
#include "thresh.h"

typedef enum {
    COMPONENT_RESISTOR,
    COMPONENT_DIODE,
    COMPONENT_INDUCTOR,
    COMPONENT_CAPACITOR,
    COMPONENT_BATTERY,
    COMPONENT_GROUND
} COMPONENT_TYPE;

typedef struct {
    uint8_t componentID;
    uint32_t xPosition;
    uint32_t yPosition;
    COMPONENT_TYPE componentType;
    
    struct componentNode *next;
} componentNode;

error_hdcr_t hdcr_run_program(
    char* inputImageFileName,
    char* outputImageFileName,
    bool adaptiveThreshold,
    ADAPTIVE_THRESHOLD_TYPE att,
    uint8_t inputThreshold,
    uint8_t MOV, //MaxOutputValue
    uint8_t CGL, // ComponentGrayLevel
    bool verbose,
    bool write,
    char* imgType
    );

#endif /* HDCR_H */
