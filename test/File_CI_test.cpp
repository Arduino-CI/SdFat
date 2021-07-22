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
  file_ci _file;
  File_CI file(&_file);
  assertEqual(20, file.available32());
}

unittest(close) {
  file_ci _file;
  File_CI file(&_file);
  assertTrue(file);
  assertTrue(file.close());
  assertFalse(file);
  assertFalse(file.close());
}

unittest(flush) {
  file_ci _file;
  File_CI file(&_file);
  file.flush();
}

unittest(getName) {
  file_ci _file;
  File_CI file(&_file);
  char name[20];
  size_t size = file.getName(name, sizeof(name));
  assertEqual(7, size);
  assertEqual("Genesis", name);
}

unittest(name) {
  file_ci _file;
  File_CI file(&_file);
  assertEqual(String("Genesis"), file.name());
}

unittest(position_rewind_seek_seekEnd) {
  file_ci _file;
  File_CI file(&_file);
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
  file_ci _file;
  File_CI file(&_file);
  assertEqual((int)'I', file.read());
  assertEqual((int)'n', file.read());
}

unittest(readBytes) {
  file_ci _file;
  File_CI file(&_file);
  char bytes[20];
  size_t size = file.read(bytes, 6);
  bytes[6] = 0;
  assertEqual(6, size);
  assertEqual("In the", bytes);
}

unittest(size) {
  file_ci _file;
  File_CI file(&_file);
  assertEqual(20, file.size());
}

unittest(truncate_at_position) {
  file_ci _file;
  File_CI file(&_file);
  assertEqual(20, file.size());
  assertEqual(0, file.position());
  file.seek(10);
  assertEqual(10, file.position());
  assertTrue(file.truncate());
  assertEqual(10, file.size());
  assertEqual(10, file.position());
}

unittest(truncate_at_value) {
  file_ci _file;
  File_CI file(&_file);
  assertEqual(20, file.size());
  assertEqual(0, file.position());
  assertTrue(file.truncate(10));
  assertEqual(10, file.size());
  assertEqual(10, file.position());
}

unittest(write_string) {
  file_ci _file;
  File_CI file(&_file);
  assertEqual(20, file.size());
  assertEqual(0, file.position());
  assertTrue(file.seekEnd());
  assertEqual(8, file.write(" created"));
  assertEqual(28, file.size());
  assertEqual(28, file.position());
}

unittest_main()
