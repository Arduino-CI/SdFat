#include "SD_CI.h"

#ifdef MOCK_PINS_COUNT

// expected by the SdFat library but not supplied by Arduino-CI
char *__brkval = nullptr;
char __bss_end = 0;

bool SdFat_CI::begin(uint8_t csPin) {
  _didBegin = true;
  return true;
}

bool SdFat_CI::format() { return true; }

#endif
