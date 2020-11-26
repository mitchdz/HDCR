#include "libhdcr.h"

/* helper function to easily decode error message */
struct _errordesc{
    int code;
    char *message;
} errordesc[] = {
    { E_hdcr_SUCCESS,  (char *)"No error" },
    { E_hdcr_FILE_NOT_FOUND, (char *)"File not found" },
    { E_hdcr_GENERIC_ERROR,  (char *)"Generic Error" },
    { E_hdcr_ARRAY_SIZE_MISMATCH,  (char *)"Array sizes do not match" },
    { E_hdcr_NOT_IMPLEMENTED,  (char *)"Not implemented yet" },
    { E_hdcr_RECURSIVE_FIRST_LAST_NOT_FOUND, (char *)"Recursive first and last index not able to be found"},
    { E_hdcr_NOT_IMPLEMENTED, (char *)"Not implemented"},
};

void printError(error_hdcr_t E, char *msg)
{
    printf("Error %d: %s %s\n", errordesc[E].code, errordesc[E].message ,msg);
}
