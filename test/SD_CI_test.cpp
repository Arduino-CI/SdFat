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

unittest_setup() {
  sd.begin(0);
  sd.format();
}

unittest_teardown() { sd.format(); }

unittest(begin) { assertTrue(sd.begin(0)); }

unittest(directory_operations) {
  assertEqual("/", sd._getCwd());
  assertTrue(sd.chdir());
  assertEqual("/", sd._getCwd());
  assertTrue(sd.chdir("/"));
  assertEqual("/", sd._getCwd());
  assertFalse(sd.chdir("/foo"));
  assertEqual("/", sd._getCwd());

  assertFalse(sd.mkdir(""));                // already exists
  assertFalse(sd.mkdir("/"));               // already exists
  assertFalse(sd.mkdir("/foo/bar", false)); // no intermediate directory
  assertTrue(sd.mkdir("/foo/bar"));         // create intermediate directory
  assertTrue(sd.exists("/"));
  assertTrue(sd.exists("/foo"));
  assertTrue(sd.exists("/foo/"));
  assertTrue(sd.exists("/foo/bar"));
  assertTrue(sd.exists("/foo/bar/"));

  assertEqual("/", sd._getCwd());
  assertTrue(sd.chdir("/foo"));
  assertEqual("/foo/", sd._getCwd());
  assertTrue(sd.chdir("bar"));
  assertEqual("/foo/bar/", sd._getCwd());

  assertFalse(sd.rmdir("/foo"));
  assertTrue(sd.exists("/foo"));
  assertTrue(sd.exists("/foo/bar"));
  assertEqual("/foo/bar/", sd._getCwd());
  assertTrue(sd.rmdir("/foo/bar"));
  assertTrue(sd.rmdir("/foo"));
  assertEqual("/", sd._getCwd());
}

unittest(fail_if_file_does_not_exist) {
  File_CI file = sd.open("foo.txt");
  assertFalse(file);
}

unittest(format) { assertTrue(sd.format()); }

unittest_main()
