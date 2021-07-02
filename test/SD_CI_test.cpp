/*
bundle config --local path vendor/bundle
bundle install
bundle exec arduino_ci.rb  --skip-examples-compilation
bundle exec arduino_ci.rb  --skip-unittests
 */
#include "Arduino.h"
#include "ArduinoUnitTests.h"
#include "SD.h"

unittest_setup() {
  SD sd;
  sd.format();
}

unittest_teardown() {
  SD sd;
  sd.format();
}

unittest(begin) {
  SD sd;
  assertTrue(sd.begin(0));
}

unittest(format) {
  SD sd;
  assertTrue(sd.format());
}

unittest_main()
