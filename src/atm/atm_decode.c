#include "atm/atm_decode.h"
#include "atm/atm_header.h"
#include "crypto/hc128.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

extern const char *kTrackNames[5];

void DecodeATM(char *filename) {
    FILE *atm_file = fopen(filename, "rb");

    if (!atm_file){
        ATM_ERROR("Could not open file '%s'", filename);
    }

    ATMHeader atm_header;
    if (fread(&atm_header, sizeof(ATMHeader), 1, atm_file) != 1){
        ATM_ERROR("ATM header malformed");
    }

    if (atm_header.num_tracks != 5){
        ATM_ERROR("Only ATM files with 5 tracks are supported");
    }

    uint32_t chunk_size = atm_header.chunk_size;

    // Total number of bytes across all tracks
    uint32_t total_bytes = GetATMTrackBytes(&atm_header);

    // Fetch the secret key
    uint32_t secret_key[4];
    GetATMSecretKey(&atm_header, secret_key);

    uint32_t max_len = GetATMMaxTrackLength(&atm_header);
    uint32_t last_chunk = max_len / chunk_size; // Last chunk before we switch chunk sizing

    for (uint32_t i = 0; i < atm_header.num_tracks; ++i){
        uint32_t track_len = atm_header.track_bytes[i];

        // Calculate the initialization vector
        uint8_t iv[16];
        GetATMInitVector(&atm_header, iv, i);

        // Seek to the beginning of the track chunk 
        fseek(atm_file,
            sizeof(ATMHeader) + i * chunk_size,
            SEEK_SET);

        uint32_t bytes_read = 0;
        uint32_t chunks_read = 0;

        uint8_t *track_data = malloc(track_len);
        uint32_t read_len = chunk_size;

        /* Each track is segmented sequentially in chunks specified by 
         * 'chunk_size' in the header. For example if the chunk size is
         * 400, track 1 will contain 400 bytes, and then the next 400 bytes
         * will belong to track 2, so on and so forth. */
        while (bytes_read < track_len){
            uint32_t stream_pos = bytes_read;

            if (chunks_read == last_chunk){
                read_len = max_len % chunk_size;
            } else if (track_len - bytes_read < chunk_size){
                read_len = track_len - bytes_read;
            }

            if (fread(track_data + stream_pos, read_len, 1, atm_file) != 1){
                printf("Error reading ATM track %d\n", i);
                exit(1);
            }

            chunks_read++;
            bytes_read += read_len;

            // Check if we are writing in the last block
            if (chunks_read == last_chunk){
                fseek(atm_file,
                    (atm_header.num_tracks - 1 - i) * chunk_size + (max_len % chunk_size) * (i),
                    SEEK_CUR);
            } else{
                fseek(atm_file,
                    chunk_size * (atm_header.num_tracks - 1),
                    SEEK_CUR);
            }
        }

        HC128((uint8_t *)secret_key, iv, track_data, track_data, track_len);

        // XOR first 8 bytes with total track lengths
        ((uint32_t *)track_data)[0] ^= total_bytes;
        ((uint32_t *)track_data)[1] ^= total_bytes;

        const char *track_name = kTrackNames[i];
        FILE *out_at3 = fopen(track_name, "wb");

        if (!out_at3){
            ATM_ERROR("Could not open '%s' for writing", track_name);
        }

        fwrite(track_data, track_len, 1, out_at3);
        fclose(out_at3);

        ATM_LOG("Finished writing %s", track_name);

        free(track_data);
    }

    fclose(atm_file);
}
