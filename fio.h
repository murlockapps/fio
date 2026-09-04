// $VER: fio.h V1.6 (03.09.2026)
// Copyright (C) 2026 Michael Sobol info@murlock.de
// Public Domain (PD)
//
// Overview:
//  fio.h V1.6 (03.09.2026)
//  Copyright (C) 2026 Michael Sobol info@murlock.de - Public Domain (PD)
//
//  Portable file functions for basic input and output (Linux and Windows)
//  fio is a small library for basic file operations.
//  It comes as an STB-style single-file library with no external dependencies.
//
// Features:
//  +Support for large files (64bit)
//  +All strings support utf8 encoding
//  +Same API on all platforms
//  +System specific abstractions for path separator and eol
//  +Byteorder specific read and write functions
//  +Single header library (no extra compiling is necessary)
//  +Standalone, no extra libraries needed
//
// License:
//  The fio software is Public Domain (PD).
//  This is free and unencumbered software released into the public domain.
//
//  Anyone is free to copy, modify, publish, use, compile, sell, or
//  distribute this software, either in source code form or as a compiled
//  binary, for any purpose, commercial or non-commercial, and by any means.
//
//  In jurisdictions that recognize copyright laws, the author or authors
//  of this software dedicate any and all copyright interest in the
//  software to the public domain. We make this dedication for the benefit
//  of the public at large and to the detriment of our heirs and
//  successors. We intend this dedication to be an overt act of
//  relinquishment in perpetuity of all present and future rights to this
//  software under copyright law.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//
//  For more information, please refer to <http://unlicense.org>
//
// Passed tests:
//  openSUSE Leap 15.2           -> 22.05.2023
//  Devuan GNU/Linux 3 (beowulf) -> 22.05.2023
//  Windows 10 Pro               -> 22.05.2023
//  Windows 11 Pro               -> 03.09.2026
//
// Compatible compilers:
//  g++ 8.3.0
//  TDM-GCC 9.2.0
//  TDM-GCC 10.3.0
//
// Version history:
//  V1.6 (03.09.2026):
//   Return value was wrong in fread_bytes.
//   The self-test was outsourced into fiotest.cpp.
//   Unnecessary macros removed and portable additional functions added
//   such as filePosition and fileSeek.
//  V1.5 (11.09.2025):
//   Rename functions fileLoadBytes fileSaveBytes into fread_bytes fwrite_bytes
//  V1.4 (23.08.2025):
//   New functions fread_flt, fread_dbl, fwrite_flt and fwrite_dbl.
//  V1.3 (22.05.2023):
//   New functions fread_u8 and fwrite_u8.
//   Changed behaviour if file pointer is invalid.
//  V1.2 (29.06.2021):
//   Compiler bugfix for windows
//   WINVER undef
//  V1.1 (15.03.2021):
//   The functions fread_u16, fread_u32 and fread_u64 have been changed.
//   They return true on success and false on fail now.
//   The documentation was revised.
//
// Feel free to contact me if there are any problems or further questions.
// Please send fio bug reports to info@murlock.de
//
#ifndef _FIO_H_
#define _FIO_H_

// Library version information
#define FIO_VER 1
#define FIO_REV 6
#define FIO_VERSTR "1.6"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <vector>

#ifdef __linux__
// Linux specific implementation
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct stat64 ststat64;

#define FIO_PATH_SEPARATOR '/'
#define FIO_EOL "\n"

#elif defined(_WIN32)
// Windows specific implementation
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#ifdef __MINGW32__
#undef __MSVCRT_VERSION__
#define __MSVCRT_VERSION__ 0x0A00
#endif

#include <tchar.h>

#ifdef WINVER
#undef WINVER
#endif
#define WINVER 0x0A00

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0A00

#include <windows.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct __stat64 ststat64;

#define FIO_PATH_SEPARATOR '\\'
#define FIO_EOL "\r\n"

// Convert a wide unicode string to an UTF8 string
static std::string wstring_to_utf8(const std::wstring &wstr) {
  if (wstr.empty()) return std::string();
  int size_needed = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                        wstr.data(), (int)wstr.size(),
                                        NULL, 0, NULL, NULL);
  if (size_needed <= 0) return std::string();
  std::string strTo(size_needed, 0);
  int size_converted = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                           wstr.data(), (int)wstr.size(),
                                           &strTo[0], size_needed, NULL, NULL);
  if (size_converted <= 0) return std::string();
  return strTo;
}

// Convert an UTF8 string to a wide unicode string
static std::wstring utf8_to_wstring(const std::string &str) {
  if (str.empty()) return std::wstring();
  int size_needed = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                        str.data(), (int)str.size(),
                                        NULL, 0);
  if (size_needed <= 0) return std::wstring();
  std::wstring wstrTo(size_needed, 0);
  int size_converted = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                           str.data(), (int)str.size(),
                                           &wstrTo[0], size_needed);
  if (size_converted <= 0) return std::wstring();
  return wstrTo;
}

#else
#error unsupported platform
#endif

// undef bswap...
#ifdef bswap_16
#undef bswap_16
#endif

#ifdef bswap_u16
#undef bswap_u16
#endif

#ifdef bswap_32
#undef bswap_32
#endif

#ifdef bswap_u32
#undef bswap_u32
#endif

#ifdef bswap_64
#undef bswap_64
#endif

#ifdef bswap_u64
#undef bswap_u64
#endif


// *************
//  DEFINITIONS
// *************

#define ENDIAN_LITTLE 0
#define ENDIAN_BIG    1

// The buffer size in bytes for catching the file input and output.
#define FILEIOBUFSIZE 8192

size_t strSize(const char *s) noexcept;
bool isBigEndian() noexcept;

int16_t  bswap_16(int16_t v) noexcept;
uint16_t bswap_u16(uint16_t v) noexcept;
int32_t  bswap_32(int32_t v) noexcept;
uint32_t bswap_u32(uint32_t v) noexcept;
int64_t  bswap_64(int64_t v) noexcept;
uint64_t bswap_u64(uint64_t v) noexcept;

bool fread_u8(FILE *fp, uint8_t &rv) noexcept;
bool fread_u16(FILE *fp, bool bBigEndian, uint16_t &rv) noexcept;
bool fread_u32(FILE *fp, bool bBigEndian, uint32_t &rv) noexcept;
bool fread_u64(FILE *fp, bool bBigEndian, uint64_t &rv) noexcept;
bool fread_flt(FILE *fp, bool bBigEndian, float &rv) noexcept;
bool fread_dbl(FILE *fp, bool bBigEndian, double &rv) noexcept;
bool fread_bytes(FILE *fp, std::vector<uint8_t> &rv, int64_t len=0) noexcept;

bool fwrite_u8(FILE *fp, uint8_t v) noexcept;
bool fwrite_u16(FILE *fp, bool bBigEndian, uint16_t v) noexcept;
bool fwrite_u32(FILE *fp, bool bBigEndian, uint32_t v) noexcept;
bool fwrite_u64(FILE *fp, bool bBigEndian, uint64_t v) noexcept;
bool fwrite_flt(FILE *fp, bool bBigEndian, float v) noexcept;
bool fwrite_dbl(FILE *fp, bool bBigEndian, double v) noexcept;
bool fwrite_bytes(FILE *fp, const std::vector<uint8_t> &v, int64_t len=0) noexcept;

FILE* fileOpen(const char *fullpath, const char *mode) noexcept;
int fileClose(FILE *fp) noexcept;
int64_t fileSize(const char *fullpath) noexcept;
int64_t fileSize(FILE *fp) noexcept;
int64_t filePosition(FILE *fp) noexcept;
int fileSeek(FILE *fp, int64_t offset, int origin) noexcept;
bool fileReadable(const char *fullpath) noexcept;
bool fileExists(const char *fullpath) noexcept;
int fileType(const char *fullpath) noexcept;
time_t fileModificationTime(const char *fullpath) noexcept;
bool fileDelete(const char *fullpath) noexcept;
bool fileRename(const char *oldpath, const char *newpath) noexcept;
bool fileCreateDirectory(const char *fullpath, int mode = 0777) noexcept;

// ****************
//  IMPLEMENTATION
// ****************

// Returns the string size in bytes from string 's'.
size_t strSize(const char *s) noexcept {
  if (!s) return 0;
  const char *p = s;
  while (*p) ++p;
  return (size_t)(p - s);
}

// Returns true on big-endian systems and false otherwise.
bool isBigEndian() noexcept {
  union {
    uint32_t i;
    char c[4];
  } bint = {0x01020304};
  return (bint.c[0] == 1);
}

// Swaps the byte order between little-endian and big-endian (int16_t)
int16_t bswap_16(int16_t v) noexcept {
  return (int16_t)bswap_u16((uint16_t)(v));
}

// Swaps the byte order between little-endian and big-endian (uint16_t)
uint16_t bswap_u16(uint16_t v) noexcept {
  return ((((v)>>8)&0xff) | (((v)&0xff)<<8));
}

// Swaps the byte order between little-endian and big-endian (int32_t)
int32_t bswap_32(int32_t v) noexcept {
  return (int32_t)bswap_u32((uint32_t)(v));
}

// Swaps the byte order between little-endian and big-endian (uint32_t)
uint32_t bswap_u32(uint32_t v) noexcept {
  return ((((v) & 0xff000000) >> 24)
          | (((v) & 0x00ff0000) >>  8) |
          (((v) & 0x0000ff00) << 8)
          | (((v) & 0x000000ff) << 24));
}

// Swaps the byte order between little-endian and big-endian (int64_t)
int64_t bswap_64(int64_t v) noexcept {
  return (int64_t)bswap_u64((uint64_t)(v));
}

// Swaps the byte order between little-endian and big-endian (uint64_t)
uint64_t bswap_u64(uint64_t v) noexcept {
  v = ((v<<8) & 0xFF00FF00FF00FF00ULL)
    | ((v>>8) & 0x00FF00FF00FF00FFULL);
  v = ((v<<16) & 0xFFFF0000FFFF0000ULL)
    | ((v>>16) & 0x0000FFFF0000FFFFULL);
  return ((v<<32) | ((v>>32) & 0xFFFFFFFFULL));
}

// Reads a single byte from the file pointer 'fp'.
// Stores the byte read from the file in the variable referenced by 'rv'.
// Returns true on success and false otherwise.
bool fread_u8(FILE *fp, uint8_t &rv) noexcept {
 if (!fp) return false;
 return (fread(&rv, sizeof(uint8_t), 1, fp) == 1);
}

// Reads an unsigned short (2 bytes) from the file pointer 'fp'.
// Stores the value read from the file in the variable referenced by 'rv'.
// If 'bBigEndian' is true, the byte order is interpreted as big-endian.
// Returns true on success and false otherwise.
bool fread_u16(FILE *fp, bool bBigEndian, uint16_t &rv) noexcept {
  if (!fp) return false;
  if (fread(&rv, sizeof(uint16_t), 1, fp) != 1) return false;
  if (isBigEndian() != bBigEndian) rv = bswap_u16(rv);
  return true;
}

// Reads an unsigned int (4 bytes) from the file pointer 'fp'.
// Stores the value read from the file in the variable referenced by 'rv'.
// If 'bBigEndian' is true, the byte order is interpreted as big-endian.
// Returns true on success and false otherwise.
bool fread_u32(FILE *fp, bool bBigEndian, uint32_t &rv) noexcept {
  if (!fp) return false;
  if (fread(&rv, sizeof(uint32_t), 1, fp) != 1) return false;
  if (isBigEndian() != bBigEndian) rv = bswap_u32(rv);
  return true;
}

// Reads an uint64_t (8 bytes) from the file pointer 'fp'.
// Stores the value read from the file in the variable referenced by 'rv'.
// If 'bBigEndian' is true, the byte order is interpreted as big-endian.
// Returns true on success and false otherwise.
bool fread_u64(FILE *fp, bool bBigEndian, uint64_t &rv) noexcept {
  if (!fp) return false;
  if (fread(&rv, sizeof(uint64_t), 1, fp) != 1) return false;
  if (isBigEndian() != bBigEndian) rv = bswap_u64(rv);
  return true;
}

// Reads a float value (4 bytes) from the file pointer 'fp'.
// Stores the value read from the file in the variable referenced by 'rv'.
// If 'bBigEndian' is true, the byte order is interpreted as big-endian.
// Returns true on success and false otherwise.
bool fread_flt(FILE *fp, bool bBigEndian, float &rv) noexcept {
  if (!fp) return false;
  union {
    float flt;
    uint32_t u32;
  } bits;
  if (!fread_u32(fp, bBigEndian, bits.u32)) {
    return false;
  }
  rv = bits.flt;
  return true;
}

// Reads a double value (8 bytes) from the file pointer 'fp'.
// Stores the value read from the file in the variable referenced by 'rv'.
// If 'bBigEndian' is true, the byte order is interpreted as big-endian.
// Returns true on success and false otherwise.
bool fread_dbl(FILE *fp, bool bBigEndian, double &rv) noexcept {
  if (!fp) return false;
  union {
    double dbl;
    uint64_t u64;
  } bits;
  if (!fread_u64(fp, bBigEndian, bits.u64)) {
    return false;
  }
  rv = bits.dbl;
  return true;
}

// Loads len bytes from the current file position into a vector of bytes.
// If len is zero, then all remaining bytes are loaded up to the end of the file.
// Returns true if all requested bytes were successfully read, otherwise false.
bool fread_bytes(FILE *fp, std::vector<uint8_t> &rv,  int64_t len /* =0 */) noexcept {
  if (!fp) return false;
  if (len < 0) return false;
  if (len == 0) {
    int64_t sz = fileSize(fp);
    if (sz < 0) return false;
    int64_t pos = filePosition(fp);
    if (pos < 0 || sz < pos) return false;
    len = sz - pos;
  }
  rv.clear();
  rv.reserve(static_cast<size_t>(len));
  int64_t remaining = len;
  uint8_t buf[FILEIOBUFSIZE];
  while(remaining > 0) {
    size_t nBytes = 0;
    if (remaining >= FILEIOBUFSIZE) {
      nBytes = fread(buf, 1, FILEIOBUFSIZE, fp);
    } else {
      nBytes = fread(buf, 1, static_cast<size_t>(remaining), fp);
    }
    if (nBytes == 0) break;
    rv.insert(rv.end(), buf, buf + nBytes);
    remaining -= static_cast<int64_t>(nBytes);
  }
  return remaining == 0;
}

// Writes a single byte value 'v' to the file pointer 'fp'.
// Returns true on success and false otherwise.
bool fwrite_u8(FILE *fp, uint8_t v) noexcept {
  if (!fp) return false;

  if (1 != fwrite(&v, sizeof(uint8_t), 1, fp)) {
    return false;
  }

  return true;
}

// Writes an unsigned short (2 bytes) value 'v' to file pointer 'fp'.
// If 'bBigEndian' is true, the value is written in big-endian byte order.
// Returns true on success and false otherwise.
bool fwrite_u16(FILE *fp, bool bBigEndian, uint16_t v) noexcept {
  if (!fp) return false;

  if (isBigEndian() != bBigEndian) {
    v = bswap_u16(v);
  }
  if (1 != fwrite(&v, sizeof(uint16_t), 1, fp)) {
    return false;
  }

  return true;
}

// Writes an unsigned int (4 bytes) value 'v' to file pointer 'fp'.
// If 'bBigEndian' is true, the value is written in big-endian byte order.
// Returns true on success and false otherwise.
bool fwrite_u32(FILE *fp, bool bBigEndian, uint32_t v) noexcept {
  if (!fp) return false;

  if (isBigEndian() != bBigEndian) {
    v = bswap_u32(v);
  }
  if (1 != fwrite(&v, sizeof(uint32_t), 1, fp)) {
    return false;
  }

  return true;
}

// Writes an uint64_t (8 bytes) value 'v' to file pointer 'fp'.
// If 'bBigEndian' is true, the value is written in big-endian byte order.
// Returns true on success and false otherwise.
bool fwrite_u64(FILE *fp, bool bBigEndian, uint64_t v) noexcept {
  if (!fp) return false;

  if (isBigEndian() != bBigEndian) {
    v = bswap_u64(v);
  }
  if (1 != fwrite(&v, sizeof(uint64_t), 1, fp)) {
    return false;
  }

  return true;
}

// Writes a float (4 bytes) value 'v' to file pointer 'fp'.
// If 'bBigEndian' is true, the value is written in big-endian byte order.
// Returns true on success and false otherwise.
bool fwrite_flt(FILE *fp, bool bBigEndian, float v) noexcept {
  union {
    float flt;
    uint32_t u32;
  } bits;
  bits.flt = v;
  return fwrite_u32(fp, bBigEndian, bits.u32);
}


// Writes a double (8 bytes) value 'v' to file pointer 'fp'.
// If 'bBigEndian' is true, the value is written in big-endian byte order.
// Returns true on success and false otherwise.
bool fwrite_dbl(FILE *fp, bool bBigEndian, double v) noexcept {
  union {
    double dbl;
    uint64_t u64;
  } bits;
  bits.dbl = v;
  return fwrite_u64(fp, bBigEndian, bits.u64);
}

// Saves 'len' bytes from the given vector v into file fp.
// If 'len' is zero, then the whole vector is written to the file.
// Returns true if all requested bytes were successfully written, otherwise false.
bool fwrite_bytes(FILE *fp, const std::vector<uint8_t> &v, int64_t len /* =0 */) noexcept {
  if (!fp) return false;
  if (len < 0) return false;
  if (len == 0 || static_cast<size_t>(len) > v.size()) {
    len = static_cast<int64_t>(v.size());
  }
  size_t n = 0;
  uint8_t buf[FILEIOBUFSIZE];
  while(len > 0) {
    size_t nBufferBytes   = 0;
    size_t nBytesWritten  = 0;
    if (len >= FILEIOBUFSIZE) {
      nBufferBytes = FILEIOBUFSIZE;
    } else {
      nBufferBytes = (size_t)len;
    }
    memcpy(buf, &v[n], nBufferBytes);
    n += nBufferBytes;
    nBytesWritten = fwrite(buf, 1, nBufferBytes, fp);
    if (nBufferBytes != nBytesWritten) {
      return false;
    }
    len -= nBytesWritten;
  }
  return true;
}

// Opens a file in 64-bit mode
FILE* fileOpen(const char *fullpath, const char *mode) noexcept {
#ifdef __linux__
  return fopen64(fullpath, mode);
#elif defined(_WIN32)
  std::wstring wpath = utf8_to_wstring(fullpath);
  std::wstring wmode = utf8_to_wstring(mode);
  return _wfopen(wpath.c_str(), wmode.c_str());
#endif
}

// Closes a file 'fp'
int fileClose(FILE *fp) noexcept {
  return fp ? fclose(fp) : EOF;
}

// Returns the size of a given file 'fullpath' in bytes or -1 on errors.
int64_t fileSize(const char *fullpath) noexcept {
  if (strSize(fullpath) == 0) return -1;
  ststat64 st_buf;
#ifdef __linux__
  int rc = stat64(fullpath, &st_buf);
#elif defined(_WIN32)
  std::wstring wpath = utf8_to_wstring(fullpath);
  int rc = _wstat64(wpath.c_str(), &st_buf);
#endif
  return (rc == 0 ? st_buf.st_size : -1);
}

// Returns the size of a given file descriptor 'fp' in bytes or -1 on errors.
int64_t fileSize(FILE *fp) noexcept {
  if (!fp) return -1;
  ststat64 st_buf;
#ifdef __linux__
  int rc = fstat64(fileno(fp), &st_buf);
#elif defined(_WIN32)
  int rc = _fstat64(fileno(fp), &st_buf);
#endif
  return (rc == 0 ? st_buf.st_size : -1);
}

// Returns the current file position in fp or -1 on errors.
int64_t filePosition(FILE *fp) noexcept {
if (!fp) return -1;
#ifdef __linux__
  return ftello64(fp);
#elif defined(_WIN32)
  return _ftelli64(fp);
#endif
}

// Sets the file position in fp according to offset and origin.
// Returns -1 on errors.
int fileSeek(FILE *fp, int64_t offset, int origin) noexcept {
  if (!fp) return -1;
#ifdef __linux__
  return fseeko64(fp, offset, origin);
#elif defined(_WIN32)
  return _fseeki64(fp, offset, origin);
#endif
}

// Returns true if file 'fullpath' or directory is readable.
bool fileReadable(const char *fullpath) noexcept {
  if (strSize(fullpath) == 0) return false;
#ifdef __linux__
  if (access(fullpath, R_OK) != 0) return false;
#elif defined(_WIN32)
  std::wstring wpath = utf8_to_wstring(fullpath);
  if (_waccess(wpath.c_str(), R_OK) != 0) return false;
#endif
  return true;
}

// Returns true if given file 'fullpath' exists, otherwise false.
// Files can exist but are not readable.
bool fileExists(const char *fullpath) noexcept {
  if (strSize(fullpath) == 0) return false;
  ststat64 st_buf;
  int rc;
#ifdef __linux__
  rc = stat64(fullpath, &st_buf);
#elif defined(_WIN32)
  std::wstring wpath = utf8_to_wstring(fullpath);
  rc = _wstat64(wpath.c_str(), &st_buf);
#else
  rc = -1;
#endif
  return (rc == 0);
}

// Returns the type of a file.
// (-1=error, 0=file, 1=directory, 2=symlink)
int fileType(const char *fullpath) noexcept {
  if (strSize(fullpath) == 0) return -1;
  ststat64 st_buf;
#ifdef __linux__
  int rc = lstat64(fullpath, &st_buf);
#elif defined(_WIN32)
  std::wstring wpath = utf8_to_wstring(fullpath);
  int rc = _wstat64(wpath.c_str(), &st_buf);
#endif
  if (rc != 0) return -1;
  if (S_ISREG(st_buf.st_mode)) return 0; // regular file
  if (S_ISDIR(st_buf.st_mode)) return 1; // directory
#ifdef __linux__
  if (S_ISLNK(st_buf.st_mode)) return 2; // symbolic link
#endif
  return -1;
}

// Returns the modification time of a file 'fullpath'
time_t fileModificationTime(const char *fullpath) noexcept {
  if (strSize(fullpath) == 0) return 0;
  ststat64 st_buf;
  int rc = 0;
#ifdef __linux__
  rc = stat64(fullpath, &st_buf);
#elif defined(_WIN32)
  std::wstring wpath = utf8_to_wstring(fullpath);
  rc = _wstat64(wpath.c_str(), &st_buf);
#endif
  return (rc == 0) ? st_buf.st_mtime : 0;
}

// Deletes a file 'fullpath'
bool fileDelete(const char *fullpath) noexcept {
  if (strSize(fullpath) == 0) return false;
  bool ret = false;
#ifdef __linux__
  ret = (unlink(fullpath) != -1);
#elif defined(_WIN32)
  std::wstring wpath = utf8_to_wstring(fullpath);
  ret = (_wunlink(wpath.c_str()) != -1);
#endif
  return ret;
}

// Renames a file from 'oldpath' into 'newpath'
bool fileRename(const char *oldpath, const char *newpath) noexcept {
  if (strSize(oldpath) == 0 || strSize(newpath) == 0) return false;
#ifdef __linux__
  return (rename(oldpath, newpath) == 0);
#elif defined(_WIN32)
  std::wstring woldpath = utf8_to_wstring(oldpath);
  std::wstring wnewpath = utf8_to_wstring(newpath);
  return (_wrename(woldpath.c_str(), wnewpath.c_str()) == 0);
#endif
}

// Creates a directory.
// The specific permissions are ignored on Windows.
bool fileCreateDirectory(const char *fullpath, int mode /* = 0777 */) noexcept {
  if (strSize(fullpath) == 0) return false;
#ifdef __linux__
  return (mkdir(fullpath, mode) == 0);
#elif defined(_WIN32)
  std::wstring wpath = utf8_to_wstring(fullpath);
  return (_wmkdir(wpath.c_str()) == 0);
#endif
}

#endif
// EOF
