#include "SD_CI.h"

File_CI::File_CI(String name, String contents) {
  this->_name = name;
  this->_contents = contents;
}

uint32_t File_CI::available32() const {
  return this->size() - this->position();
}

bool File_CI::close() { return true; }

void File_CI::flush() {}

size_t File_CI::getName(char *name, size_t size) {
  strncpy(name, _name.c_str(), size);
  return min(size, _name.size());
}

const char *File_CI::name() const { return _name.c_str(); }

uint32_t File_CI::position() const { return _position; }

int File_CI::read() {
  if (this->available32()) {
    return static_cast<int>(_contents.at(_position++));
  } else {
    return -1;
  }
}

int File_CI::read(void *buf, size_t count) {
  int size = min(count, this->available32());
  strncpy(static_cast<char *>(buf), _contents.c_str() + _position, size);
  _position += size;
  return size;
}

void File_CI::rewind() { _position = 0; }

bool File_CI::seek(uint32_t pos) {
  _position = pos;
  return true;
}

bool File_CI::seekEnd(int32_t offset) {
  this->seek(this->size() + offset);
  return true;
}

uint32_t File_CI::size() const { return _contents.size(); }

bool File_CI::truncate() {
  _contents.remove(_position);
  return true;
}

bool File_CI::truncate(uint32_t length) {
  _contents.remove(length);
  _position = length;
  return true;
}

size_t File_CI::write(const char *str) {
  size_t size = strlen(str);
  std::cout << '"' << str << "\" size = " << size << std::endl;
  _contents.remove(_position);
  _contents.concat(str);
  _position += size;
  return size;
}
