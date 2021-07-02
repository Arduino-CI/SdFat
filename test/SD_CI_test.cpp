/*
bundle config --local path vendor/bundle
bundle install
bundle exec arduino_ci.rb  --skip-examples-compilation
bundle exec arduino_ci.rb  --skip-unittests
 */
#include "Arduino.h"
#include "ArduinoUnitTests.h"
#include "SD.h"

SD sd;

unittest_setup() { sd.format(); }

unittest_teardown() { sd.format(); }

unittest(begin) { assertTrue(sd.begin(0)); }

unittest(format) { assertTrue(sd.format()); }

unittest_main()
