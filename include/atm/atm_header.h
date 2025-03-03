#ifndef ATM_HEADER_H
#define ATM_HEADER_H

#include <stdint.h>

typedef struct {
    uint32_t key4; // 0x0
    uint32_t num_tracks; // 0x4
    uint32_t key2; // 0x8
    uint32_t chunk_size; // 0xC
    uint32_t key1; // 0x10
    uint32_t track_bytes[5]; // 0x14
    uint32_t word10; // 0x28
    uint32_t word11; // 0x2C
    uint32_t word12; // 0x30
    uint32_t key3; // 0x34
  } ATMHeader;

  uint32_t GetATMTrackBytes(ATMHeader *atm);
  uint32_t GetATMMaxTrackLength(ATMHeader *atm);

  void GetATMSecretKey(ATMHeader *atm, uint32_t *key_arr);
  void GetATMInitVector(ATMHeader *atm, uint8_t *iv, uint32_t track);

#endif
