#include "atm/atm_encode.h"
#include "crypto/hc128.h"
#include "utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern const char *kTrackNames[5];

static void InitHeader(ATMHeader *header) {
    header->num_tracks = 5;
    header->chunk_size = 400;

    header->key1 = 0xDEADBEEF;
    header->key2 = 0xDEADBEEF;
    header->key3 = 0xDEADBEEF;
    header->key4 = 0xDEADBEEF;

    header->word10 = 0xDEADBEEF;
    header->word11 = 0xDEADBEEF;
    header->word12 = 0xDEADBEEF;
}


void EncodeATM(char *filename){
    // Create the header 
    ATMHeader atm_header;
    AT3File atracs[5];

    InitHeader(&atm_header);

    uint32_t total_bytes = 0;
    uint32_t max_length = 0;

    for (int i = 0; i < 5; ++i){
        uint32_t file_len;
        FILE *file = fopen(kTrackNames[i], "rb");

        if (!file){
            ATM_ERROR("Could not create/open '%s', for writing", kTrackNames[i]);
        }

        fseek(file, 0, SEEK_END); // seek to end of file
        file_len = ftell(file); // get current file pointer
        fseek(file, 0, SEEK_SET); // seek back to beginning of file

        // Setup ATRAC context
        atracs[i].file_size = file_len;
        atracs[i].data = malloc(file_len);
        atracs[i].bytes_written = 0;

        // Correct header data
        atm_header.track_bytes[i] = file_len;

        // Read in ATRAC data
        if (!fread(atracs[i].data, file_len, 1, file)){
            ATM_ERROR("Malformed data in %s", kTrackNames[i]);
        }

        fclose(file);

        if (file_len > max_length){
            max_length = file_len;
        }

        total_bytes += file_len;
    }

    uint32_t secret_key[4];
    GetATMSecretKey(&atm_header, secret_key);

    for (int i = 0; i < 5; i++){
        uint8_t iv[16];
        GetATMInitVector(&atm_header, iv, i);

        // XOR the first 8 bytes to complete the decryption
        ((uint32_t *)atracs[i].data)[0] ^= total_bytes;
        ((uint32_t *)atracs[i].data)[1] ^= total_bytes;

        HC128((uint8_t *)secret_key,
            iv,
            atracs[i].data,
            atracs[i].data,
            atracs[i].file_size);
    }

    FILE *out_atm = fopen(filename, "wb");

    fwrite(&atm_header, sizeof(ATMHeader), 1, out_atm);

    uint32_t chunks_written = 0;
    uint32_t total_bytes_written = 0;

    uint8_t *temp_chunk = malloc(atm_header.chunk_size);
    uint32_t last_chunk = total_bytes / 400;
    uint32_t write_size = atm_header.chunk_size;
    while (total_bytes_written < total_bytes){
        if(chunks_written > last_chunk){
            write_size = max_length % 400;
        }

        for(int i = 0; i < 5; i++){
            uint32_t bytes_written = atracs[i].bytes_written;
            uint32_t file_size = atracs[i].file_size;

            if(bytes_written >= file_size){ // Case for track finished (write padding)
                memset(temp_chunk, 0, write_size);
            } else if (file_size - bytes_written < atm_header.chunk_size) { // Case for end of track (copy remaining bytes)
                uint32_t remaining_len = file_size - bytes_written;
                memset(temp_chunk, 0, write_size);
                memcpy(temp_chunk, atracs[i].data + bytes_written, remaining_len);

                atracs[i].bytes_written += remaining_len;
                total_bytes_written += remaining_len;
                chunks_written++;
            } else { // Regular case
                memcpy(temp_chunk, atracs[i].data + bytes_written, write_size);

                atracs[i].bytes_written += write_size;
                total_bytes_written += write_size;
                chunks_written++;
            }

            fwrite(temp_chunk, write_size, 1, out_atm);
        }
        
    }

    fclose(out_atm);
    
    ATM_LOG("Finished encoding %s", filename);
}