/*
bundle config --local path vendor/bundle
bundle install
bundle exec arduino_ci.rb  --skip-examples-compilation
bundle exec arduino_ci.rb  --skip-unittests
 */
#include "Arduino.h"
#include "ArduinoUnitTests.h"
#include "SD.h"

unittest(available32) {
  File_CI file("Genesis", "In the beginning God");
  assertEqual(20, file.available32());
}

unittest(close) {
  File_CI file("Genesis", "In the beginning God");
  assertTrue(file.close());
}

unittest(flush) {
  File_CI file("Genesis", "In the beginning God");
  file.flush();
}

unittest(getName) {
  File_CI file("Genesis", "In the beginning God");
  char name[20];
  size_t size = file.getName(name, sizeof(name));
  assertEqual(7, size);
  assertEqual("Genesis", name);
}

unittest(name) {
  File_CI file("Genesis", "In the beginning God");
  assertEqual(String("Genesis"), file.name());
}

unittest(position_rewind_seek_seekEnd) {
  File_CI file("Genesis", "In the beginning God");
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
  File_CI file("Genesis", "In the beginning God");
  assertEqual((int)'I', file.read());
  assertEqual((int)'n', file.read());
}

unittest(readBytes) {
  File_CI file("Genesis", "In the beginning God");
  char bytes[20];
  size_t size = file.read(bytes, 6);
  assertEqual(6, size);
  assertEqual("In the", bytes);
}

unittest(size) {
  File_CI file("Genesis", "In the beginning God");
  assertEqual(20, file.size());
}

unittest(truncate_at_position) {
  File_CI file("Genesis", "In the beginning God");
  assertEqual(20, file.size());
  assertEqual(0, file.position());
  file.seek(10);
  assertEqual(10, file.position());
  assertTrue(file.truncate());
  assertEqual(10, file.size());
  assertEqual(10, file.position());
}

unittest(truncate_at_value) {
  File_CI file("Genesis", "In the beginning God");
  assertEqual(20, file.size());
  assertEqual(0, file.position());
  assertTrue(file.truncate(10));
  assertEqual(10, file.size());
  assertEqual(10, file.position());
}

unittest(write_string) {
  File_CI file("Genesis", "In the beginning God");
  assertEqual(20, file.size());
  assertEqual(0, file.position());
  assertTrue(file.seekEnd());
  assertEqual(8, file.write(" created"));
  assertEqual(28, file.size());
  assertEqual(28, file.position());
}

unittest_main()
