#include <Arduino.h>

#ifdef MOCK_PINS_COUNT
#include "SD_CI.h"
#define SD SdFat_CI
#else
#include "SdFat.h"
#define SD SdFat
#endif
