#include "atm/atm_header.h"
#include <string.h>
#include <stdio.h>

const char *kTrackNames[5] = {"drums.at3",
                              "bass_guitar.at3",
                              "lead_guitar.at3",
                              "vocals.at3",
                              "fill.at3"};


uint32_t GetATMTrackBytes(ATMHeader *atm) {
    uint32_t total_bytes = 0;

    for (uint32_t i = 0; i < atm->num_tracks; ++i){
        total_bytes += atm->track_bytes[i];
    }

    return total_bytes;
}

uint32_t GetATMMaxTrackLength(ATMHeader *atm) {
  uint32_t max_bytes = 0;

  for (uint32_t i = 0; i < atm->num_tracks; ++i){
    if (atm->track_bytes[i] > max_bytes){
      max_bytes = atm->track_bytes[i];
    }
  }

  return max_bytes;
}


void GetATMSecretKey(ATMHeader *atm, uint32_t *key_arr) {
    key_arr[0] = atm->key1 ^ 0x5156B456;
    key_arr[1] = atm->key2 ^ 0x28CBD70B;
    key_arr[2] = atm->key3 ^ 0x4B5AEFFC;
    key_arr[3] = atm->key4 ^ 0x6165E84B;
}

void GetATMInitVector(ATMHeader *atm, uint8_t *iv, uint32_t track) {
  uint32_t num_tracks = atm->num_tracks;
  uint32_t unk = ((8 - num_tracks) * 4) / num_tracks + 1;

  // Copy the bytes back into a char array to avoid compiler warnings
  uint8_t header_bytes[sizeof(ATMHeader)];
  memcpy(header_bytes, atm, sizeof(ATMHeader));

  for (int i = 0; i < 16; ++i){
    // Grabs a byte from the ATM header
    uint8_t atm_byte = *(uint8_t *)(header_bytes +
        ((track * unk + i % unk) - track) + 
        num_tracks * 4 + 0x14);
        
    // We XOR the new IV byte with the last one
    uint8_t last_byte = (i == 0) ? 0 : iv[i - 1];

    // Install the IV byte
    iv[i] = atm_byte ^ last_byte + ~atm_byte + (uint8_t)i + (uint8_t)track;
  }
}