#include <Arduino.h>

#ifdef MOCK_PINS_COUNT

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

struct file_ci {
  file_ci(String name, const char *contents = "") {
    this->name = name;
    this->size = strlen(contents);
    this->contents = static_cast<uint8_t *>(malloc(size));
    this->position = 0;
    memcpy(this->contents, contents, this->size);
  }
  ~file_ci() {
    free(this->contents);
    this->contents = nullptr;
  }
  uint8_t *contents;
  String name;
  uint32_t position = 0;
  size_t size;
};

class File_CI {
public:
  File_CI(file_ci *file);
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

private:
  String _cwd = String("/");
  bool _didBegin = false;
  std::set<String> dirs = {"/"};
  String _normalizeDirPath(String inPath);
  String _normalizeFilePath(String inPath);
};

#endif
