#include "SD_CI.h"

#ifdef MOCK_PINS_COUNT

#include <cassert>

File_CI::File_CI() {}

File_CI::File_CI(file_ci *file) { this->file = file; }

uint32_t File_CI::available32() const {
  assert(file != nullptr);
  return this->size() - this->position();
}

bool File_CI::close() {
  bool flag = file != nullptr;
  file = nullptr;
  return flag;
}

void File_CI::flush() { assert(file != nullptr); }

size_t File_CI::getName(char *name, size_t size) {
  assert(file != nullptr);
  strncpy(name, file->name.c_str(), size);
  return min(size, file->name.length());
}

const char *File_CI::name() const {
  assert(file != nullptr);
  return file->name.c_str();
}

uint32_t File_CI::position() const {
  assert(file != nullptr);
  return file->position;
}

int File_CI::read() {
  assert(file != nullptr);
  if (this->available32()) {
    uint8_t *ptr = static_cast<uint8_t *>(file->contents + file->position);
    int value = *ptr;
    ++(file->position);
    return value;
  } else {
    return -1;
  }
}

int File_CI::read(void *buf, size_t count) {
  assert(file != nullptr);
  int size = min(count, this->available32());
  memcpy(buf, file->contents + file->position, size);
  file->position += size;
  return size;
}

void File_CI::rewind() {
  assert(file != nullptr);
  file->position = 0;
}

bool File_CI::seek(uint32_t pos) {
  assert(file != nullptr);
  file->position = pos;
  return true;
}

bool File_CI::seekEnd(int32_t offset) {
  assert(file != nullptr);
  this->seek(this->size() + offset);
  return true;
}

uint32_t File_CI::size() const {
  assert(file != nullptr);
  return file->size;
}

bool File_CI::truncate() {
  assert(file != nullptr);
  uint8_t *ptr = static_cast<uint8_t *>(malloc(file->position));
  if (!ptr) {
    return false;
  }
  memcpy(ptr, file->contents, file->position);
  free(file->contents);
  file->contents = ptr;
  file->size = file->position;
  return true;
}

bool File_CI::truncate(uint32_t length) {
  assert(file != nullptr);
  file->position = length;
  return this->truncate();
}

size_t File_CI::write(const char *str) {
  return this->write(static_cast<const void *>(str), strlen(str));
}

size_t File_CI::write(const void *buf, size_t count) {
  assert(file != nullptr);
  if (count <= this->available32()) {
    memcpy(file->contents + file->position, buf, count);
  } else {
    uint8_t *ptr = static_cast<uint8_t *>(malloc(file->position + count));
    if (!ptr) {
      return -1;
    }
    memcpy(ptr, file->contents, file->position);
    free(file->contents);
    file->contents = ptr;
    file->size = file->position + count;
  }
  file->position += count;
  return count;
}

File_CI::operator bool() { return file != nullptr; }

#endif
