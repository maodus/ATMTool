#ifndef ATM_ENCODE_H
#define ATM_ENCODE_H

#include "atm/atm_header.h"

typedef struct {
    uint8_t *data;
    uint32_t file_size;
    uint32_t bytes_written;
} AT3File;

void EncodeATM(char *filename);
#endif