#include "SD_CI.h"

#ifdef MOCK_PINS_COUNT

#include <cassert>

// expected by the SdFat library but not supplied by Arduino-CI
char *__brkval = nullptr;
char __bss_end = 0;

String SdFat_CI::_normalizePath(String inPath) {
  String outPath;
  if (inPath.length() == 0 || inPath == "/") {
    return String("/");
  }
  if (inPath.at(0) == '/') {
    outPath = inPath;
  } else {
    outPath = _cwd;
    outPath.concat(inPath);
  }
  if (!outPath.endsWith("/")) {
    outPath.concat('/');
  }
  return outPath;
}

String SdFat_CI::_getCwd() { return _cwd; }

bool SdFat_CI::begin(uint8_t csPin) {
  _didBegin = true;
  return true;
}

bool SdFat_CI::chdir() {
  assert(_didBegin);
  _cwd = String("/");
  return true;
}

bool SdFat_CI::chdir(const char *path) { return this->chdir(String(path)); }

bool SdFat_CI::chdir(const String &path) {
  assert(_didBegin);
  String newPath = this->_normalizePath(path);
  if (dirs.count(newPath)) {
    _cwd = newPath;
    return true;
  }
  return false;
}

bool SdFat_CI::exists(const char *path) { return this->exists(String(path)); }

bool SdFat_CI::exists(const String &path) {
  assert(_didBegin);
  if (dirs.count(this->_normalizePath(path))) {
    return true;
  }
  return false;
}

bool SdFat_CI::format() {
  assert(_didBegin);
  dirs.clear();
  dirs.emplace("/");
  return this->chdir();
}

bool SdFat_CI::ls(uint8_t flags) { return false; }

bool SdFat_CI::mkdir(const char *path, bool pFlag) {
  return this->mkdir(String(path), pFlag);
}

bool SdFat_CI::mkdir(const String &path, bool pFlag) {
  assert(_didBegin);
  String newPath = this->_normalizePath(path);
  if (dirs.count(newPath)) {
    return false;
  }
  int i = newPath.indexOf('/', 1);
  while (i > 0 && i < newPath.length()) {
    if (!dirs.count(newPath)) {
      if (pFlag) {
        dirs.emplace(newPath.substring(0, i + 1));
      } else {
        return false;
      }
    }
    i = newPath.indexOf('/', i + 1);
  }
  dirs.emplace(newPath);
  return true;
}

// File_CI SdFat_CI::open(const char *path, oflag_t oflag = 0x00);

// File_CI SdFat_CI::open(const String &path, oflag_t oflag = 0x00);

bool SdFat_CI::remove(const char *path) { return this->remove(String(path)); }

bool SdFat_CI::remove(const String &path) { return false; }

bool SdFat_CI::rmdir(const char *path) { return this->rmdir(String(path)); }

bool SdFat_CI::rmdir(const String &path) {
  assert(_didBegin);
  String newPath = this->_normalizePath(path);
  if (!dirs.count(newPath)) {
    return false;
  }
  for (auto dir : dirs) {
    if (dir != newPath && dir.startsWith(newPath)) {
      return false;
    }
  }
  if (_cwd == newPath) {
    this->chdir();
  }
  dirs.erase(newPath);
  return true;
}

#endif
