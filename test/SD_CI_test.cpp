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
File file;

unittest_setup() {
  sd.begin(0);
  sd.format();
}

unittest_teardown() {
  file.close();
  sd.format();
}

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

unittest(format) {
  assertTrue(sd.mkdir("/foo/bar"));
  assertTrue(sd.exists("/foo"));
  assertTrue(sd.exists("/foo/bar"));
  assertTrue(sd.format());
  assertFalse(sd.exists("/foo"));
  assertFalse(sd.exists("/foo/bar"));
}

unittest(file_exists) {
  assertFalse(sd.exists("foo.txt"));
  file = sd.open("foo.txt", O_WRONLY | O_CREAT);
  assertTrue(sd.exists("foo.txt"));
}

unittest(ls) { assertTrue(true); } // TODO

/*
#define O_RDONLY 0x00 ///< Open for reading only.
#define O_WRONLY 0x01 ///< Open for writing only.
#define O_RDWR 0x02   ///< Open for reading and writing.
#define O_AT_END 0x04 ///< Open at EOF.
#define O_APPEND 0x08 ///< Set append mode.
#define O_CREAT 0x10  ///< Create file if it does not exist.
#define O_TRUNC 0x20  ///< Truncate file to zero length.
#define O_EXCL 0x40   ///< Fail if the file exists.
*/
unittest(open_readonly_fails_if_file_does_not_exist) {
  file = sd.open("foo.txt");
  assertFalse(file);
}

unittest(open_write_create) {
  file = sd.open("foo.txt", O_WRONLY | O_CREAT);
  assertTrue(file);
}

unittest(remove) { assertTrue(true); } // TODO

unittest(rename) { assertTrue(true); } // TODO

unittest_main()
