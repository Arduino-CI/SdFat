#include <Arduino.h>

#ifdef MOCK_PINS_COUNT
#include "ArduinoCI/SD_CI.h"
#define SD SdFat_CI
#define File File_CI
#else
#include "SdFat.h"
#define SD SdFat
#endif
