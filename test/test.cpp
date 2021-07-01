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
  SD *sd = new SD;
  sd->format();
}

unittest_teardown() {
  SD *sd = new SD;
  sd->format();
}

unittest(format) {
  SD *sd = new SD;
  sd->format();
}

/*
unittest(exists) {
  assertFalse(SdFat32.exists("file.txt"));
  File file = SdFat32.open("file.txt", FILE_WRITE);
  file.close();
  assertTrue(SdFat32.exists("file.txt"));
}

unittest(mkdir) {
  assertTrue(SdFat32.mkdir("test_directory"));
  assertTrue(SdFat32.mkdir("test_directory/a"));
  assertTrue(SdFat32.mkdir("test_directory/a/b"));
  assertTrue(SdFat32.mkdir("test_directory/a/c"));
  assertTrue(SdFat32.exists("test_directory"));
  assertTrue(SdFat32.exists("test_directory/a/b"));
  assertTrue(SdFat32.exists("test_directory/a/c"));
}

unittest(open) {
  // create file
  File writeFile = SdFat32.open("file.txt", FILE_WRITE);
  writeFile.close();
  // open file for read should exist
  File readFile2 = SdFat32.open("file.txt", FILE_READ);
  assertTrue(readFile2.isOpen());
  readFile2.close();
}

unittest(close) {
  // close write file
  File file = SdFat32.open("file.txt", FILE_WRITE);
  assertTrue(file.isOpen());
  file.close();
  assertFalse(file.isOpen());

  // close read file
  File readFile = SdFat32.open("file.txt", FILE_READ);
  assertTrue(readFile.isOpen());
  readFile.close();
  assertFalse(readFile.isOpen());
}

unittest(remove) {
  // set up
  File file = SdFat32.open("file.txt", FILE_WRITE);
  file.close();
  assertTrue(SdFat32.exists("file.txt"));

  SdFat32.remove("file.txt");
  assertFalse(SdFat32.exists("file.txt"));
}

unittest(rmdir) {
  // set up
  SdFat32.mkdir("test_directory");
  SdFat32.mkdir("test_directory/a");
  SdFat32.mkdir("test_directory/a/a");
  SdFat32.mkdir("test_directory/a/b");
  SdFat32.mkdir("test_directory/a/c");
  File file = SdFat32.open("test_directory/a/a/file.txt", FILE_WRITE);
  file.close();

  // remove directory
  assertTrue(SdFat32.rmdir("test_directory/a/c"));
  // make sure non-removed dirs still exist
  assertTrue(SdFat32.exists("test_directory"));
  assertTrue(SdFat32.exists("test_directory/a/a"));
  // make sure removed dir no longer exists
  assertFalse(SdFat32.exists("test_directory/a/c"));
}

unittest(name) {
  // set up
  File file = SdFat32.open("newFile.txt", FILE_WRITE);
  assertEqual("newFile.txt", file.name());
  file.close();

  SdFat32.mkdir("dir");
  file = SdFat32.open("dir");
  assertEqual("dir", file.name());
  file.close();
}

unittest(seek) {
  // set up
  File file = SdFat32.open("seek.txt", FILE_WRITE);
  char write[] = "Hide and Seek.";
  file.write(write, sizeof(write) - 1);
  file.close();
  File read = SdFat32.open("seek.txt", FILE_READ);

  // Testing
  char readFrom[4];
  char expected[] = "and";
  read.seek(5);
  read.read(readFrom, 3);
  readFrom[3] = '\0';
  assertEqual(expected, readFrom);
}

unittest(read) {
  // set up
  File file = SdFat32.open("birthday.txt", FILE_WRITE);
  char toWrite[] = "Happy Birthday to You!";
  file.write(toWrite, sizeof(toWrite) - 1);
  file.close();

  File file2 = SdFat32.open("lines.txt", FILE_WRITE);
  char toWrite2[] = "line 1\nline2";
  file2.write(toWrite2, sizeof(toWrite2) - 1);
  file2.close();

  // testing
  File readFile = SdFat32.open("birthday.txt", FILE_READ);
  size_t size = readFile.size();
  char readFromFile[size + 1];
  readFile.read(readFromFile, size);
  readFromFile[size] = '\0';
  readFile.close();

  // assertEqual(toWrite, readFromFile);

  File readFile2 = SdFat32.open("lines.txt", FILE_READ);
  char readFromFile2[7 + 1];
  char readFromFile3[5 + 1];
  readFile2.read(readFromFile2, 7);
  readFile2.read(readFromFile3, 5);
  readFromFile2[7] = '\0';
  readFromFile3[5] = '\0';

  char expected2[] = "line 1\n";
  char expected3[] = "line2";
  assertEqual(expected2, readFromFile2);
  assertEqual(expected3, readFromFile3);
}

unittest(write) {
  // open new file for writing
  File writeFile = SdFat32.open("wood.txt", FILE_WRITE);
  char toWrite[] = "How much wood could a wood pecker peck?\n";
  writeFile.write(toWrite, sizeof(toWrite) - 1);

  // read from same write file
  size_t size = writeFile.size();
  char readFromFile[size + 1];
  writeFile.seek(0);
  writeFile.read(readFromFile, size);
  readFromFile[size] = '\0';
  // assertEqual(toWrite, readFromFile);
  writeFile.close();

  // open old writing file to write at end.
  File writeFile2 = SdFat32.open("wood.txt", FILE_WRITE);
  char toWrite2[] = "A lot of wood.\n";
  writeFile2.write(toWrite2, sizeof(toWrite2) - 1);
  writeFile2.close();

  // check old writing file
  File readWrite2 = SdFat32.open("wood.txt", FILE_READ);
  size_t size2 = readWrite2.size();
  char toRead2[size2 + 1];
  readWrite2.read(toRead2, size2);
  toRead2[size] = '\0';
  char expected2[] =
      "How much wood could a wood pecker peck?\nA lot of wood.\n";
  // assertEqual(expected2, toRead2);
  readWrite2.close();
}

unittest(size) {
  // setup
  File sizeFile = SdFat32.open("size.txt", FILE_WRITE);
  char toWrite[] = "Test text\n";
  sizeFile.write(toWrite, sizeof(toWrite) - 1);
  sizeFile.close();

  // test
  uint32_t fileSize = sizeFile.size();
  assertEqual(10, fileSize);
}

unittest(peek) {
  // set up
  File peekFile = SdFat32.open("peek.txt", FILE_WRITE);
  char toWrite[] = "Peek file content\n";
  peekFile.write(toWrite, sizeof(toWrite) - 1);
  peekFile.close();

  // Test
  File readPeek = SdFat32.open("peek.txt", FILE_READ);
  assertEqual('P', readPeek.peek());
  assertEqual('P', readPeek.peek());
  readPeek.close();

  File readWritePeek = SdFat32.open("peek.txt", FILE_WRITE);
  readWritePeek.seek(0);
  assertEqual('P', readWritePeek.peek());
  assertEqual('P', readWritePeek.peek());
  readWritePeek.close();
}

unittest(position) {
  // set up
  File posFile = SdFat32.open("pos.txt", FILE_WRITE);
  char toWrite[] = "This is the position file.\n";
  posFile.write(toWrite, sizeof(toWrite) - 1);

  // test
  assertEqual(27, posFile.position());
  posFile.seek(5);
  assertEqual(5, posFile.position());
  posFile.close();
}

unittest(iSdFat32irectory) {
  // set up
  SdFat32.mkdir("test");
  File toRead = SdFat32.open("read.txt", FILE_WRITE);
  toRead.close();
  File testFile = SdFat32.open("test.txt", FILE_WRITE);
  File readFile = SdFat32.open("read.txt", FILE_READ);

  // test
  File myDir = SdFat32.open("test");
  assertTrue(myDir.iSdFat32irectory());
  assertFalse(testFile.iSdFat32irectory());
  assertFalse(readFile.iSdFat32irectory());

  // tear down
  testFile.close();
  readFile.close();
}
*/

unittest_main()
