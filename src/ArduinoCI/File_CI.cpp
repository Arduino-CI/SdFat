#include "SD_CI.h"

#ifdef MOCK_PINS_COUNT

#include <cassert>

file_ci::file_ci(String name) {
  this->name = name;
  this->contents = static_cast<uint8_t *>(malloc(0));
  this->size = 0;
}

file_ci::~file_ci() { free(this->contents); }

/*
#define O_RDONLY 0x00 ///< Open for reading only.
#define O_WRONLY 0x01 ///< Open for writing only.
#define O_RDWR 0x02   ///< Open for reading and writing.
#define O_AT_END 0x04 ///< Open at EOF.
#define O_APPEND 0x08 ///< Set append mode.
#define O_TRUNC 0x20  ///< Truncate this->file to zero length.
*/
File_CI::File_CI(file_ci *file, oflag_t oflag) {
  this->file = file;
  this->oflag = oflag;
  if (oflag & O_TRUNC) {
    if (this->file->size) {
      free(this->file->contents);
      this->file->contents = static_cast<uint8_t *>(malloc(0));
      this->file->size = 0;
    }
    this->_position = 0;
  } else if (oflag & O_AT_END) {
    this->_position = this->size();
  }
}

uint32_t File_CI::available32() const {
  return this->size() - this->position();
}

bool File_CI::close() {
  bool flag = this->file != nullptr;
  this->file = nullptr;
  return flag;
}

void File_CI::flush() { assert(this->file != nullptr); }

size_t File_CI::getName(char *name, size_t size) {
  assert(this->file != nullptr);
  strncpy(name, this->file->name.c_str(), size);
  return min(size, this->file->name.length());
}

const char *File_CI::name() const {
  assert(this->file != nullptr);
  return this->file->name.c_str();
}

uint32_t File_CI::position() const {
  assert(this->file != nullptr);
  return this->_position;
}

int File_CI::read() {
  assert(this->file != nullptr);
  if (this->oflag == O_WRONLY) {
    return -1;
  }
  if (this->available32()) {
    uint8_t *ptr =
        static_cast<uint8_t *>(this->file->contents + this->_position);
    int value = *ptr;
    ++(this->_position);
    return value;
  } else {
    return -1;
  }
}

int File_CI::read(void *buf, size_t count) {
  assert(this->file != nullptr);
  if ((this->oflag & O_ACCMODE) == O_WRONLY) {
    return -1;
  }
  int size = min(count, this->available32());
  memcpy(buf, this->file->contents + this->_position, size);
  this->_position += size;
  return size;
}

void File_CI::rewind() {
  assert(this->file != nullptr);
  this->_position = 0;
}

bool File_CI::seek(uint32_t pos) {
  assert(this->file != nullptr);
  this->_position = pos;
  return true;
}

bool File_CI::seekEnd(int32_t offset) {
  assert(this->file != nullptr);
  this->seek(this->size() + offset);
  return true;
}

uint32_t File_CI::size() const {
  assert(this->file != nullptr);
  return this->file->size;
}

bool File_CI::truncate() {
  assert(this->file != nullptr);
  uint8_t *ptr = static_cast<uint8_t *>(malloc(this->_position));
  if (!ptr) {
    return false;
  }
  memcpy(ptr, this->file->contents, this->_position);
  free(this->file->contents);
  this->file->contents = ptr;
  this->file->size = this->_position;
  return true;
}

bool File_CI::truncate(uint32_t length) {
  assert(this->file != nullptr);
  this->_position = length;
  return this->truncate();
}

size_t File_CI::write(const char *str) {
  return this->write(static_cast<const void *>(str), strlen(str));
}

size_t File_CI::write(const void *buf, size_t count) {
  assert(this->file != nullptr);
  if ((this->oflag & O_ACCMODE) == O_RDONLY) {
    return -1;
  }
  if (this->oflag & O_APPEND) {
    this->seekEnd();
  }
  if (this->available32() < count) {
    uint8_t *ptr = static_cast<uint8_t *>(malloc(this->_position + count));
    if (!ptr) {
      return -1;
    }
    memcpy(ptr, this->file->contents, this->_position);
    free(this->file->contents);
    this->file->contents = ptr;
    this->file->size = this->_position + count;
  }
  memcpy(this->file->contents + this->_position, buf, count);
  this->_position += count;
  return count;
}

File_CI::operator bool() { return this->file != nullptr; }

#endif
