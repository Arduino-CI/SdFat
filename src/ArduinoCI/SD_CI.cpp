#include "SD_CI.h"

// expected by the SdFat library but not supplied by Arduino-CI
char *__brkval = nullptr;
char __bss_end = 0;

bool SdFat_CI::begin(uint8_t csPin) { return false; }

bool SdFat_CI::format() { return false; }
