#include "SD_CI.h"

#ifdef MOCK_PINS_COUNT

File_CI::File_CI(file_ci *file) { this->file = file; }

uint32_t File_CI::available32() const {
  return this->size() - this->position();
}

bool File_CI::close() { return true; }

void File_CI::flush() {}

size_t File_CI::getName(char *name, size_t size) {
  strncpy(name, file->name.c_str(), size);
  return min(size, file->name.length());
}

const char *File_CI::name() const { return file->name.c_str(); }

uint32_t File_CI::position() const { return file->position; }

int File_CI::read() {
  if (this->available32()) {
    return static_cast<int>(file->contents.at(file->position++));
  } else {
    return -1;
  }
}

int File_CI::read(void *buf, size_t count) {
  int size = min(count, this->available32());
  memcpy(static_cast<char *>(buf), file->contents.c_str() + file->position,
         size);
  file->position += size;
  return size;
}

void File_CI::rewind() { file->position = 0; }

bool File_CI::seek(uint32_t pos) {
  file->position = pos;
  return true;
}

bool File_CI::seekEnd(int32_t offset) {
  this->seek(this->size() + offset);
  return true;
}

uint32_t File_CI::size() const { return file->contents.length(); }

bool File_CI::truncate() {
  file->contents.remove(file->position);
  return true;
}

bool File_CI::truncate(uint32_t length) {
  file->contents.remove(length);
  file->position = length;
  return true;
}

size_t File_CI::write(const char *str) {
  size_t size = strlen(str);
  std::cout << '"' << str << "\" size = " << size << std::endl;
  file->contents.remove(file->position);
  file->contents.concat(str);
  file->position += size;
  return size;
}

#endif
