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
  file = sd.open("Genesis", O_CREAT | O_WRONLY);
  file.write("In the beginning God");
  file.rewind();
}

unittest_teardown() {
  file.close();
  sd.format();
}

unittest(append) {
  file.close();
  file = sd.open("Genesis", O_APPEND | O_WRONLY);
  file.write(" created the heavens");
  file.rewind();
  file.write(" and the earth.");
  file.close();
  file = sd.open("Genesis", O_RDONLY);
  assertEqual(55, file.size());
  char bytes[60];
  size_t size = file.read(bytes, 60);
  assertEqual(55, size);
  bytes[55] = 0;
  assertEqual("In the beginning God created the heavens and the earth.", bytes);
}

unittest(available32) {
  assertEqual(0, file.position());
  assertEqual(20, file.size());
  assertEqual(20, file.available32());
}

unittest(close) {
  assertTrue(file);
  assertTrue(file.close());
  assertFalse(file);
  assertFalse(file.close());
}

unittest(flush) { file.flush(); }

unittest(getName) {
  char name[20];
  size_t size = file.getName(name, sizeof(name));
  assertEqual(7, size);
  assertEqual("Genesis", name);
}

unittest(name) { assertEqual(String("Genesis"), file.name()); }

unittest(position_rewind_seek_seekEnd) {
  assertEqual(0, file.position());
  file.seek(10);
  assertEqual(10, file.position());
  file.rewind();
  assertEqual(0, file.position());
  file.seekEnd();
  assertEqual(20, file.position());
  file.seekEnd(-1);
  assertEqual(19, file.position());
}

unittest(readByte) {
  assertEqual((int)'I', file.read());
  assertEqual((int)'n', file.read());
}

unittest(readBytes) {
  char bytes[20];
  size_t size = file.read(bytes, 6);
  bytes[6] = 0;
  assertEqual(6, size);
  assertEqual("In the", bytes);
}

unittest(size) { assertEqual(20, file.size()); }

unittest(truncate_at_position) {
  assertEqual(20, file.size());
  assertEqual(0, file.position());
  file.seek(10);
  assertEqual(10, file.position());
  assertTrue(file.truncate());
  assertEqual(10, file.size());
  assertEqual(10, file.position());
}

unittest(truncate_at_value) {
  assertEqual(20, file.size());
  assertEqual(0, file.position());
  assertTrue(file.truncate(10));
  assertEqual(10, file.size());
  assertEqual(10, file.position());
}

unittest(write_string) {
  assertEqual(20, file.size());
  assertEqual(0, file.position());
  assertTrue(file.seekEnd());
  assertEqual(8, file.write(" created"));
  assertEqual(28, file.size());
  assertEqual(28, file.position());
}

unittest_main()
