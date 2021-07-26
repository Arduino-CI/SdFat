#include <Arduino.h>

#ifdef MOCK_PINS_COUNT

#include <map>
#include <set>

const uint8_t LS_A = 0x01;    // ls() flag for list all files including hidden.
const uint8_t LS_DATE = 0x02; // ls() flag to print modify date.
const uint8_t LS_SIZE = 0x04; // ls() flag to print file size.
const uint8_t LS_R = 0x08;    // ls() flag for recursive list of subdirectories.

#define O_RDONLY 0x00 ///< Open for reading only.
#define O_WRONLY 0x01 ///< Open for writing only.
#define O_RDWR 0x02   ///< Open for reading and writing.
#define O_AT_END 0x04 ///< Open at EOF.
#define O_APPEND 0x08 ///< Set append mode.
#define O_CREAT 0x10  ///< Create file if it does not exist.
#define O_TRUNC 0x20  ///< Truncate file to zero length.
#define O_EXCL 0x40   ///< Fail if the file exists.
#define O_SYNC 0x80   ///< Synchronized write I/O operations.

#define O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR) ///< Mask for access mode.
typedef uint8_t oflag_t;

// private file
class file_ci {
public:
  file_ci(String name);
  ~file_ci();
  uint8_t *contents = nullptr;
  String name;
  uint32_t size = 0;
};

// public wrapper for a file with API matching File[32]
class File_CI {
public:
  File_CI(file_ci *file = nullptr, oflag_t oflag = 0x00);
  uint32_t available32() const;
  bool close();
  void flush();
  size_t getName(char *name, size_t size);
  const char *name() const;
  uint32_t position() const;
  int read();
  int read(void *buf, size_t count);
  void rewind();
  bool seek(uint32_t pos);
  bool seekEnd(int32_t offset = 0);
  uint32_t size() const;
  bool truncate();
  bool truncate(uint32_t length);
  size_t write(const char *str);
  size_t write(const void *buf, size_t count);
  operator bool();

private:
  file_ci *file;
  oflag_t oflag = 0x00;
  uint32_t _position = 0;
};

class SdFat_CI {
public:
  String _getCwd();
  bool begin(uint8_t csPin);
  bool chdir();
  bool chdir(const char *path);
  bool chdir(const String &path);
  bool exists(const char *path);
  bool exists(const String &path);
  bool format();
  bool ls(uint8_t flags = 0);
  bool mkdir(const char *path, bool pFlag = true);
  bool mkdir(const String &path, bool pFlag = true);
  File_CI open(const char *path, oflag_t oflag = 0x00);
  File_CI open(const String &path, oflag_t oflag = 0x00);
  bool remove(const char *path);
  bool remove(const String &path);
  bool rmdir(const char *path);
  bool rmdir(const String &path);
  bool rename(const char *oldPath, const char *newPath);
  bool rename(const String &oldPath, const String &newPath);

private:
  String _cwd = String("/");
  bool _didBegin = false;
  std::set<String> dirs = {"/"};
  std::map<String, file_ci *> files;
  String _normalizeDirPath(String inPath);
  String _normalizeFilePath(String inPath);
};

#endif
