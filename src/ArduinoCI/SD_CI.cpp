#include "SD_CI.h"

#ifdef MOCK_PINS_COUNT

#include <cassert>

// expected by the SdFat library but not supplied by Arduino-CI
char *__brkval = nullptr;
char __bss_end = 0;

String SdFat_CI::_normalizeDirPath(String inPath) {
  String outPath = _normalizeFilePath(inPath);
  if (!outPath.endsWith("/")) {
    outPath.concat('/');
  }
  return outPath;
}

String SdFat_CI::_normalizeFilePath(String inPath) {
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
  return outPath;
}

String SdFat_CI::_getCwd() { return _cwd; }

bool SdFat_CI::begin(uint8_t csPin) {
  std::cout << "SdFat_CI::begin()" << std::endl;
  return true;
}

bool SdFat_CI::chdir() {
  assert(_didBegin);
  std::cout << "SdFat_CI::chdir() - 1" << std::endl;
  _cwd = String("/");
  std::cout << "SdFat_CI::chdir() - 2" << std::endl;
  return true;
}

bool SdFat_CI::chdir(const char *path) { return this->chdir(String(path)); }

bool SdFat_CI::chdir(const String &path) {
  assert(_didBegin);
  String newPath = this->_normalizeDirPath(path);
  if (dirs.count(newPath)) {
    _cwd = newPath;
    return true;
  }
  return false;
}

bool SdFat_CI::exists(const char *path) { return this->exists(String(path)); }

bool SdFat_CI::exists(const String &path) {
  assert(_didBegin);
  if (dirs.count(this->_normalizeDirPath(path)) == 1) {
    return true;
  }
  if (files.count(this->_normalizeFilePath(path)) == 1) {
    return true;
  }
  return false;
}

bool SdFat_CI::format() {
  std::cout << "SdFat_CI::format() - 1" << std::endl;
  assert(_didBegin);
  dirs.clear();
  std::cout << "SdFat_CI::format() - 2" << std::endl;
  dirs.emplace("/");
  std::cout << "SdFat_CI::format() - 3" << std::endl;
  for (auto const &each : files) {
    delete each.second;
  }
  std::cout << "SdFat_CI::format() - 4" << std::endl;
  files.clear();
  std::cout << "SdFat_CI::format() - 5" << std::endl;
  return this->chdir();
}

bool SdFat_CI::ls(uint8_t flags) { return false; }

bool SdFat_CI::mkdir(const char *path, bool pFlag) {
  return this->mkdir(String(path), pFlag);
}

bool SdFat_CI::mkdir(const String &path, bool pFlag) {
  assert(_didBegin);
  String newPath = this->_normalizeDirPath(path);
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

File_CI SdFat_CI::open(const char *path, oflag_t oflag) {
  return open(String(path), oflag);
}

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
File_CI SdFat_CI::open(const String &path, oflag_t oflag) {
  String fullPath = _normalizeFilePath(path);
  file_ci *file = nullptr;
  if (this->exists(fullPath)) {
    if (oflag & O_EXCL) {
      return File_CI(); // empty file reference to signal an error
    }
    file = files.at(fullPath);
  } else {                 // file does not exist
    if (oflag & O_CREAT) { // create file
      file = new file_ci(path);
      files.emplace(fullPath, file);
    } else {
      return File_CI(); // empty file reference to signal an error
    }
  }
  // open existing file with proper mode
  return File_CI(file, oflag);
}

bool SdFat_CI::remove(const char *path) { return this->remove(String(path)); }

bool SdFat_CI::remove(const String &path) {
  if (this->exists(path)) {
    String fullPath = _normalizeFilePath(path);
    file_ci *file = files.at(fullPath);
    files.erase(fullPath);
    delete file;
    return true;
  }
  return false;
}

bool SdFat_CI::rename(const char *oldPath, const char *newPath) {
  return this->rename(String(oldPath), String(newPath));
}

bool SdFat_CI::rename(const String &oldPath, const String &newPath) {
  String fullOldPath = _normalizeFilePath(oldPath);
  String fullNewPath = _normalizeFilePath(newPath);
  if (!this->exists(fullOldPath) || this->exists(fullNewPath)) {
    return false;
  }
  file_ci *file = this->files.at(fullOldPath);
  file->name = fullNewPath;
  this->files.erase(fullOldPath);
  this->files.emplace(fullNewPath, file);
  return true;
}

bool SdFat_CI::rmdir(const char *path) { return this->rmdir(String(path)); }

bool SdFat_CI::rmdir(const String &path) {
  assert(_didBegin);
  String newPath = this->_normalizeDirPath(path);
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
