// $VER: fio.h V1.2 (29.06.2021)
// Copyright (C) 2021 Michael Sobol info@murlock.de - Public Domain (PD)
//
// Portable file functions for basic input and output (linux and windows)
//
//  Features:
//   +large files are supported (64bit)
//   +all strings can contain utf8 encoding
//   +same functions on all platforms (therefore also same code)
//   +fseeko64 ftello64 fopen64 fstat64 stat64 ststat64 on all systems
//   +system specific abstractions PATH_SEPARATOR EOL
//   +byteorder specific read and write functions
//   +single header
//
// passed tests:
//  openSUSE Leap 15.2           -> 29.06.2021
//  Devuan GNU/Linux 3 (beowulf) -> 29.06.2021
//  Windows 10 Pro               -> 29.06.2021
//
// license:
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
//  THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//
//  For more information, please refer to <http://unlicense.org>
//
// Feel free to contact me if there are any problems or further questions.
// Please send fio bug reports to info@murlock.de
//
#ifndef _FIO_H_
#define _FIO_H_

// library version information
#define FIO_VER 1
#define FIO_REV 2
#define FIO_VERSTR "1.2"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <vector>
#include <inttypes.h> // for selftest
// #include <algorithm>
// #include <string>

#ifdef __linux__
// ***************
// Linux specific
// ***************
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct stat64 ststat64;

#define PATH_SEPARATOR '/'
#define EOL "\n"

#elif defined(_WIN32) || defined(WIN32)
// *****************
// Windows specific
// *****************
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#if __MINGW32__
#undef __MSVCRT_VERSION__
#define __MSVCRT_VERSION__ 0x0601
#endif

#include <tchar.h>

#ifdef WINVER
#undef WINVER
#endif
#define WINVER 0x0601

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0601

#include <windows.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#define fseeko64 _fseeki64
#define ftello64 _ftelli64
#define fopen64 _wfopen
#define fstat64 _fstat64
#ifdef stat64
#undef stat64
#endif
#ifndef stat64
#define stat64 _wstat64
#endif

// stat64 vs ststat64 ?
typedef struct __stat64 ststat64;

#define PATH_SEPARATOR '\\'
#define EOL "\r\n"

// Convert a wide unicode string to an UTF8 string
static std::string wstring_to_utf8(const std::wstring &wstr) {
  if (wstr.empty()) return std::string();
  int size_needed = WideCharToMultiByte(CP_UTF8, 0,
                                        &wstr[0], (int)wstr.size(),
                                        NULL, 0, NULL, NULL);
  std::string strTo( size_needed, 0 );
  WideCharToMultiByte(CP_UTF8, 0, &wstr[0],
                      (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
  return strTo;
}

// Convert an UTF8 string to a wide unicode string
static std::wstring utf8_to_wstring(const std::string &str) {
  if (str.empty()) return std::wstring();
  int size_needed = MultiByteToWideChar(CP_UTF8, 0,
                                        &str[0], (int)str.size(),
                                        NULL, 0);
  std::wstring wstrTo( size_needed, 0 );
  MultiByteToWideChar(CP_UTF8, 0, &str[0],
                      (int)str.size(), &wstrTo[0], size_needed);
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

size_t strSize(const char *s);
bool isBigEndian(void);
int16_t  bswap_16(int16_t v);
uint16_t bswap_u16(uint16_t v);
int32_t  bswap_32(int32_t v);
uint32_t bswap_u32(uint32_t v);
int64_t  bswap_64(int64_t v);
uint64_t bswap_u64(uint64_t v);

bool fread_u16(FILE *fp, bool bBigEndian, uint16_t &rv);
bool fread_u32(FILE *fp, bool bBigEndian, uint32_t &rv);
bool fread_u64(FILE *fp, bool bBigEndian, uint64_t &rv);
bool fwrite_u16(FILE *fp, bool bBigEndian, uint16_t v);
bool fwrite_u32(FILE *fp, bool bBigEndian, uint32_t v);
bool fwrite_u64(FILE *fp, bool bBigEndian, uint64_t v);
std::vector<uint8_t> fileLoadBytes(FILE *fp, int64_t len=0);
bool fileSaveBytes(FILE *fp, std::vector<uint8_t> &v, int64_t len=0);

FILE* fileOpen(const char *fullpath, const char *mode);
int fileClose(FILE *fp);
int64_t fileSize(const char *fullpath);
int64_t fileSize(FILE *fp);
bool fileReadable(const char *fullpath);
bool fileExists(const char *fullpath);
int fileType(const char *fullpath);
time_t fileModificationTime(const char *fullpath);
bool fileDelete(const char *fullpath);

// ****************
//  IMPLEMENTATION
// ****************

// Returns string size in bytes
size_t strSize(const char *s) {
  size_t ret=0;
  if (s) {
    size_t n=0;
    while(true) {
      if (s[n]=='\0') break;
      n++;
    }
    ret=n;
  }
  return ret;
}

// Returns true on big endian computers otherwise false
bool isBigEndian(void) {
  union {
    uint32_t i;
    char c[4];
  } bint = {0x01020304};
  return (bint.c[0] == 1);
}

// Swap little endian/big endian (int16_t)
int16_t bswap_16(int16_t v) {
  return (int16_t)bswap_u16((uint16_t)(v));
}

// Swap little endian/big endian (uint16_t)
uint16_t bswap_u16(uint16_t v) {
  return ((((v)>>8)&0xff) | (((v)&0xff)<<8));
}

// Swap little endian/big endian (int32_t)
int32_t bswap_32(int32_t v) {
  return (int32_t)bswap_u32((uint32_t)(v));
}

// Swap little endian/big endian (uint32_t)
uint32_t bswap_u32(uint32_t v) {
  return ((((v) & 0xff000000) >> 24)
          | (((v) & 0x00ff0000) >>  8) |
          (((v) & 0x0000ff00) << 8)
          | (((v) & 0x000000ff) << 24));
}

// Swap little endian/big endian (int64_t)
int64_t bswap_64(int64_t v) {
  v = ((v<<8) & 0xFF00FF00FF00FF00ULL)
    | ((v>>8) & 0x00FF00FF00FF00FFULL);
  v = ((v<<16) & 0xFFFF0000FFFF0000ULL)
    | ((v>>16) & 0x0000FFFF0000FFFFULL);
  return ((v<<32) | (v>>32));
}

// Swap little endian/big endian (uint64_t)
uint64_t bswap_u64(uint64_t v) {
  v = ((v<<8) & 0xFF00FF00FF00FF00ULL)
    | ((v>>8) & 0x00FF00FF00FF00FFULL);
  v = ((v<<16) & 0xFFFF0000FFFF0000ULL)
    | ((v>>16) & 0x0000FFFF0000FFFFULL);
  return ((v<<32) | ((v>>32) & 0xFFFFFFFFULL));
}

// Read unsigned short (2 bytes) from file
bool fread_u16(FILE *fp, bool bBigEndian, uint16_t &rv) {
  if (fp) {
    uint16_t v=0;
    if (1 != fread(&v, sizeof(uint16_t), 1, fp)) {
      return false;
    }
    if (isBigEndian()!=bBigEndian) {
      v = bswap_u16(v);
    }
    rv=v;
  }
  return true;
}

// Read unsigned int (4 bytes) from file
bool fread_u32(FILE *fp, bool bBigEndian, uint32_t &rv) {
  if (fp) {
    uint32_t v=0;
    if (1 != fread(&v, sizeof(uint32_t), 1, fp)) {
      return false;
    }
    if (isBigEndian()!=bBigEndian) {
      v = bswap_u32(v);
    }
    rv=v;
  }
  return true;
}

// Read uint64_t (8 bytes) from file
bool fread_u64(FILE *fp, bool bBigEndian, uint64_t &rv) {
  if (fp) {
    uint64_t v=0;
    if (1 != fread(&v, sizeof(uint64_t), 1, fp)) {
      return false;
    }
    if (isBigEndian()!=bBigEndian) {
      v = bswap_u64(v);
    }
    rv=v;
  }
  return true;
}

// Write unsigned short (2 bytes) to file
bool fwrite_u16(FILE *fp, bool bBigEndian, uint16_t v) {
  bool ret=false;
  if (fp) {
     if (isBigEndian()!=bBigEndian) {
       v = bswap_u16(v);
     }
     if (1 == fwrite(&v, sizeof(uint16_t), 1, fp)) {
       ret=true;
     }
  }
  return ret;
}

// Write unsigned int (4 bytes) to file
bool fwrite_u32(FILE *fp, bool bBigEndian, uint32_t v) {
  bool ret=false;
  if (fp) {
     if (isBigEndian()!=bBigEndian) {
       v = bswap_u32(v);
     }
     if (1 == fwrite(&v, sizeof(uint32_t), 1, fp)) {
       ret=true;
     }
  }
  return ret;
}

// Write uint64_t (8 bytes) to file
bool fwrite_u64(FILE *fp, bool bBigEndian, uint64_t v) {
  bool ret=false;
  if (fp) {
     if (isBigEndian()!=bBigEndian) {
       v = bswap_u64(v);
     }
     if (1 == fwrite(&v, sizeof(uint64_t), 1, fp)) {
       ret=true;
     }
  }
  return ret;
}

// Loads len bytes into an vector of bytes.
// If len is zero, then the whole file is loaded up to the end of the file.
std::vector<uint8_t> fileLoadBytes(FILE *fp, int64_t len /* =0 */) {
  std::vector<uint8_t> v;
  if (len==0) {
    len = fileSize(fp);
  }
  const int64_t n = len;
  uint8_t buf[FILEIOBUFSIZE];
  while(len>0 && !feof(fp)) {
    size_t bytes = 0;
    if (len>=FILEIOBUFSIZE) {
      bytes = fread(buf, 1, FILEIOBUFSIZE, fp);
    } else {
      bytes = fread(buf, 1, len, fp);
    }
    for (size_t i=0; i<bytes; i++) {
      v.push_back(buf[i]);
    }
    bytes>0 ? len-=bytes : len=0;
  }
  if (v.size()!=(size_t)n) {
    v.clear();
  }
  return v;
}

// Saves len bytes from the given vector v into file fp.
// If len is zero, then write the whole vector into the file fp.
// Returns true if successfull, otherwise false.
bool fileSaveBytes(FILE *fp, std::vector<uint8_t> &v, int64_t len /* =0 */) {
  if (len==0 || (size_t)len>v.size()) {
    len=v.size();
  }
  uint8_t buf[FILEIOBUFSIZE];
  size_t n = 0;
  while(len>0 && !feof(fp)) {
    size_t wbytes = 0; // current bytes in buffer
    size_t bytes = 0;  // successfully written bytes
    if (len>=FILEIOBUFSIZE) {
      wbytes = FILEIOBUFSIZE;
    } else {
      wbytes = len;
    }
    // copy to buf
    for (size_t i=0; i<wbytes; i++) {
      buf[i]=v[n++];
    }
    // write buf
    bytes = fwrite(buf, 1, wbytes, fp);
    if (bytes!=wbytes) {
      return false;
    }
    len-=bytes;
  }
  return true;
}

// Opens a file in 64-bit mode
FILE* fileOpen(const char *fullpath, const char *mode) {
#ifdef __linux__
  return fopen64(fullpath, mode);
#elif defined(_WIN32) || defined(WIN32)
  return _wfopen(utf8_to_wstring(fullpath).c_str(),
		 utf8_to_wstring(mode).c_str());
#endif
}

// Closes a file
int fileClose(FILE *fp) {
  int ret=0;
  if (fp==0){
    ret=EOF;
  } else {
    ret=fclose(fp);
  }
  return ret;
}

// Returns the size of a given file in bytes or -1 on errors.
int64_t fileSize(const char *fullpath) {
  if (strSize(fullpath)==0) return -1;
  ststat64 st_buf;
#ifdef __linux__
  size_t rc = stat64(fullpath, &st_buf);
#elif defined(_WIN32) || defined(WIN32)
  size_t rc = stat64(utf8_to_wstring(fullpath).c_str(), &st_buf);
#endif
  return (rc == 0 ? st_buf.st_size : -1);
}

// Returns the size of a given file descriptor in bytes or -1 on errors.
int64_t fileSize(FILE *fp) {
  ststat64 st_buf;
  size_t rc = fstat64(fileno(fp), &st_buf);
  return (rc == 0 ? st_buf.st_size : -1);
}

// Returns true if file or directory is readable
bool fileReadable(const char *fullpath) {
  if (strSize(fullpath)==0) return false;
#ifdef __linux__
  if (access(fullpath, R_OK) != 0) return false;
#elif defined(_WIN32) || defined(WIN32)
  if (_waccess(utf8_to_wstring(fullpath).c_str(), 4) != 0) return false;
#endif
  return true;
}

// Returns true if file exits, otherwise false
bool fileExists(const char *fullpath) {
  bool ret=false;
  if (strSize(fullpath)>0) {
    ststat64 st_buf;
    int rc;
#ifdef __linux__
    rc = stat64(fullpath, &st_buf);
#elif defined(_WIN32) || defined(WIN32)
    rc = stat64(utf8_to_wstring(fullpath).c_str(), &st_buf);
#else
    rc = -1;
#endif
    ret = (rc==0);
  }
  return ret;
}

// Returns the type of a file
// (-1=error, 0=file, 1=directory, 2=symlink)
int fileType(const char *fullpath) {
  if (strSize(fullpath)==0) return -1;
  ststat64 st_buf;
#ifdef __linux__
  int rc = stat64(fullpath, &st_buf);
#elif defined(_WIN32) || defined(WIN32)
  int rc = stat64(utf8_to_wstring(fullpath).c_str(), &st_buf);
#endif

  if (rc == 0) {
    if (S_ISREG(st_buf.st_mode)) {
      rc=0; // regular file
    } else if (S_ISDIR(st_buf.st_mode)) {
      rc=1; // directory
    } else {
#ifdef __linux__
      if (S_ISLNK(st_buf.st_mode)) {
        rc=2; // symbolic link
      }
#endif
      // S_ISLNK is not defined in windows!
    }
  } else {
    rc=-1;
  }
  return rc;
}

// Returns the modification time of a file
time_t fileModificationTime(const char *fullpath) {
  time_t ret=0;
  if (strSize(fullpath)==0) return ret;
  ststat64 st_buf;
#ifdef __linux__
  int rc = stat64(fullpath, &st_buf);
#elif defined(_WIN32) || defined(WIN32)
  int rc = stat64(utf8_to_wstring(fullpath).c_str(), &st_buf);
#endif
  if (rc == 0) {
    ret=st_buf.st_mtime;
  }
  return ret;
}

// Deletes a file
bool fileDelete(const char *fullpath) {
  bool ret=false;
  if (strSize(fullpath)==0) return ret;
#ifdef __linux__
  ret=(unlink(fullpath) != -1);
#elif defined(_WIN32) || defined(WIN32)
  ret=(_wunlink(utf8_to_wstring(fullpath).c_str()) != -1);
#endif
  return ret;
}


// ***************
// Selftest
// ***************

// For selftest add -DSELFTEST to the compiler
#ifdef SELFTEST
void fioPerr(int m=0) {
  switch(m) {
  case 0:  fprintf(stderr, "fioSelftest:"); break;
  case 1:  fprintf(stdout, "fioSelftest [PASSED]\n"); break;
  case 2:  fprintf(stderr, "fioSelftest [FAILED]\n"); break;
  }
}
bool fioSelftest() {
  bool isOk = true;
  {
    // check version information
    int exp_val=1;
    if (FIO_VER!=exp_val) {
      fioPerr();
      fprintf(stderr, " Error: FIO_VER is not %d\n", exp_val);
      isOk=false;
    }
    exp_val=2;
    if (FIO_REV!=exp_val) {
      fioPerr();
      fprintf(stderr, " Error: FIO_REV is not %d\n", exp_val);
      isOk=false;
    }
    const size_t SS=4;
    const char se[SS]="1.2"; // expected value
    const char sv[SS]=FIO_VERSTR; // real value
    for (size_t i=0; i<SS; i++) {
      if (sv[i]!=se[i]) {
        fioPerr();
        fprintf(stderr, " Error: FIO_VERSTR is not '%s'\n", se);
        isOk=false;
        break;
      }
    }
  }
  {
    // check PATH_SEPARATOR and EOL
#ifdef __linux__
    const char ce = '/';
    const char *se = "\n";
#elif defined(_WIN32) || defined(WIN32)
    const char ce = '\\';
    const char *se = "\r\n";
#endif
    if (ce!=PATH_SEPARATOR) {
      fioPerr();
      fprintf(stderr, " Error: PATH_SEPARATOR is not '%c'\n", ce);
      isOk=false;
    }
    const char *sv=EOL;
    if ( (se[0]!=sv[0]) || (se[1]!=sv[1])) {
      fioPerr();
#ifdef __linux__
      fprintf(stderr, " Error: EOL is not '\\n'\n");
#elif defined(_WIN32) || defined(WIN32)
      fprintf(stderr, " Error: EOL is not '\\r\\n'\n");
#endif
      isOk=false;
    }
  }
  {
    // check ENDIAN_LITTLE ENDIAN_BIG and FILEIOBUFSIZE
    if (false != ENDIAN_LITTLE) {
      fioPerr();
      fprintf(stderr, " Error: ENDIAN_LITTLE is not 0\n");
      isOk=false;
    }
    if (true != ENDIAN_BIG) {
      fioPerr();
      fprintf(stderr, " Error: ENDIAN_LITTLE is not 1\n");
      isOk=false;
    }
    int ie=8192;
    if (ie != FILEIOBUFSIZE) {
      fioPerr();
      fprintf(stderr, " Error: FILEIOBUFSIZE is not %d\n", ie);
      isOk=false;
    }
  }
  {
    // check strSize
    if (0!=strSize(0) || 0!=strSize(NULL)) {
      fioPerr();
      fprintf(stderr, " Error: strSize(0) is not 0\n");
      isOk=false;
    }
    if (0!=strSize("")) {
      fioPerr();
      fprintf(stderr, " Error: strSize(\"\") is not 0\n");
      isOk=false;
    }
    if (0!=strSize("\0")) {
      fioPerr();
      fprintf(stderr, " Error: strSize(\"\\0\") is not 0\n");
      isOk=false;
    }
    if (3!=strSize("foo")) {
      fioPerr();
      fprintf(stderr, " Error: strSize(\"foo\") is not 3\n");
      isOk=false;
    }
    if (0!=strSize("\0foo")) {
      fioPerr();
      fprintf(stderr, " Error: strSize(\"\\0foo\") is not 0\n");
      isOk=false;
    }
  }
  {
    // skip -> check isBigEndian
  }
  {
    // bswap_u16
    uint16_t s=0x1122;
    if (0x2211!=bswap_u16(s)) {
      fioPerr();
      fprintf(stderr, " Error: bswap_u16(0x1122) is incorrect\n");
      isOk=false;
    }
    if (s!=bswap_u16(bswap_u16(s))) {
      fioPerr();
      fprintf(stderr, " Error: bswap_u16(bswap_u16(0x1122)) is incorrect\n");
      isOk=false;
    }
    if (0!=bswap_u16(0)) {
      fioPerr();
      fprintf(stderr, " Error: bswap_u16(0) is incorrect\n");
      isOk=false;
    }
  }
  {
    // bswap_u32
    uint32_t s=0x11223344;
    if (0x44332211!=bswap_u32(s)) {
      fioPerr();
      fprintf(stderr, " Error: bswap_u32(0x11223344) is incorrect\n");
      isOk=false;
    }
    if (0!=bswap_u32(0)) {
      fioPerr();
      fprintf(stderr, " Error: bswap_u32(0) is incorrect\n");
      isOk=false;
    }
  }
  {
    // bswap_u64
  }
  {
    // open file that does not exist in read mode
    const char *fname="fiotst.dat";
    if (fileExists(fname)) {
      fioPerr();
      fprintf(stderr, " Error: fileExists(\"%s\") file exists on start\n", fname);
      isOk=false;
    } else {
      FILE *fp=fileOpen(fname, "rb");
      if (fp) {
        fioPerr();
        fprintf(stderr, " Error: fileOpen(\"%s\") falsely succeeded\n", fname);
        isOk=false;
      }
      // check existance
      if (fileExists(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileExists(\"%s\") falsely succeeded\n", fname);
        isOk=false;
      }
      time_t rawtime;
      time(&rawtime);
      // open file that does not exist in write mode
      fp=fileOpen(fname, "wb");
      if (!fp) {
        fioPerr();
        fprintf(stderr, " Error: fileOpen(\"%s\") failed\n", fname);
        isOk=false;
      }
      fileClose(fp);
      // check existance
      if (!fileExists(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileExists(\"%s\") failed\n", fname);
        isOk=false;
      }
      // file readable
      if (!fileReadable(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileReadable(\"%s\") failed\n", fname);
        isOk=false;
      }
      // file type (0=normal file)
      if (0!=fileType(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileType(\"%s\") is wrong\n", fname);
        isOk=false;
      }
      // check file size
      if (0!=fileSize(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileSize(\"%s\") is not 0\n", fname);
        isOk=false;
      }
      // check modification time
      time_t mtime=fileModificationTime(fname);
      if (mtime==0) {
        fioPerr();
        fprintf(stderr, " Error: fileModificationTime(\"%s\") failed\n", fname);
        isOk=false;
      }
      time_t tdiff=abs(mtime-rawtime);
      if (tdiff!=0) {
        fioPerr();
        fprintf(stderr, " Error: fileModificationTime(\"%s\") is inaccurate\n"
                , fname);
        isOk=false;
      }
      // delete file
      if (!fileDelete(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileDelete(\"%s\") failed\n", fname);
        isOk=false;
      }
      // check existance
      if (fileExists(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileExists(\"%s\") file exists after delete\n"
                , fname);
        isOk=false;
      }
      // null string into fileExists
      if (fileExists(0)) {
        fioPerr();
        fprintf(stderr, " Error: fileExists(0) null string file exists\n");
        isOk=false;
      }
      // check file type from deleted file
      if (-1!=fileType(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileType(\"%s\") is not -1 (file does not exist)\n"
                , fname);
        isOk=false;
      }
      // check file type from null string
      if (-1!=fileType(0)) {
        fioPerr();
        fprintf(stderr, " Error: fileType(0) is not -1\n");
        isOk=false;
      }
      // check file type from empty string
      if (-1!=fileType("")) {
        fioPerr();
        fprintf(stderr, " Error: fileType("") is not -1\n");
        isOk=false;
      }
      // file close from 0
      if (EOF!=fileClose(0)) {
        fioPerr();
        fprintf(stderr, " Error: fileClose(0) is not EOF\n");
        isOk=false;
      }
      // reopen file fname in write mode
      fp=fileOpen(fname, "wb");
      if (!fp) {
        fioPerr();
        fprintf(stderr, " Error: fileOpen(\"%s\") reopen failed\n", fname);
        isOk=false;
      }
      uint16_t u16_val=0x2030;
      // write little endian uint16_t
      if (!fwrite_u16(fp, ENDIAN_LITTLE, u16_val)) {
        fioPerr();
        fprintf(stderr, " Error: fwrite_u16 ENDIAN_LITTLE failed\n");
        isOk=false;
      }
      // write big endian uint16_t
      if (!fwrite_u16(fp, ENDIAN_BIG, u16_val)) {
        fioPerr();
        fprintf(stderr, " Error: fwrite_u16 ENDIAN_BIG failed\n");
        isOk=false;

      }
      fileClose(fp);
      // check file size
      if (4!=fileSize(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileSize(\"%s\") is not 4\n", fname);
        isOk=false;
      }
      // open for read
      fp=fileOpen(fname, "rb");
      if (!fp) {
        fioPerr();
        fprintf(stderr, " Error: fileOpen(\"%s\") reopen for read failed\n", fname);
        isOk=false;
      }
      uint16_t val=0;
      if (!fread_u16(fp, ENDIAN_LITTLE, val)) {
        fioPerr();
        fprintf(stderr, " Error: fread_u16 ENDIAN_LITTLE failed\n");
        isOk=false;
      }
      if (val!=u16_val) {
        fioPerr();
        fprintf(stderr, " Error: fread_u16 ENDIAN_LITTLE wrong value\n");
        isOk=false;
      }
      if (!fread_u16(fp, ENDIAN_LITTLE, val)) {
        fioPerr();
        fprintf(stderr, " Error: fread_u16 next ENDIAN_LITTLE failed\n");
        isOk=false;
      }
      if (bswap_u16(val)!=u16_val) {
        fioPerr();
        fprintf(stderr, " Error: fread_u16 next ENDIAN_LITTLE wrong value\n");
        isOk=false;
      }
      rewind(fp);
      uint32_t val2=0;
      if (!fread_u32(fp, ENDIAN_LITTLE, val2)){
        fioPerr();
        fprintf(stderr, " Error: fread_u32 ENDIAN_LITTLE failed\n");
        isOk=false;
      }
      if (val2!=0x30202030) {
        fioPerr();
        fprintf(stderr, " Error: fread_u32 ENDIAN_LITTLE is wrong\n");
        isOk=false;
      }
      fileClose(fp);
      // delete file
      if (!fileDelete(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileDelete(\"%s\") failed (delete second time)\n"
                , fname);
        isOk=false;
      }
      // open for read
      fp=fileOpen(fname, "wb");
      if (!fp) {
        fioPerr();
        fprintf(stderr, " Error: fileOpen(\"%s\") reopen (2) for write failed\n"
                , fname);
        isOk=false;
      }
      std::vector<uint8_t> vbuf;
      for (size_t i=0; i<16; i++) {
        vbuf.push_back(i);
      }
      if (!fileSaveBytes(fp, vbuf)) {
        fioPerr();
        fprintf(stderr, " Error: fileSaveBytes failed\n");
        isOk=false;
      }
      fileClose(fp);
      // check file size
      if (16!=fileSize(fname)) {
        fioPerr();
        fprintf(stderr, " Error: fileSize(\"%s\") is not 16\n", fname);
        isOk=false;
      }
      fp=fileOpen(fname, "rb");
      if (fp) {
        uint64_t val3=0;
        if (!fread_u64(fp, ENDIAN_LITTLE, val3)) {
          fioPerr();
          fprintf(stderr, " Error: fread_u64 ENDIAN_LITTLE is wrong\n");
          isOk=false;
        }
        if (val3!=0x0706050403020100) {
          fioPerr();
          fprintf(stderr, " Error: fread_u64 ENDIAN_LITTLE is wrong\n");
          isOk=false;
        }
        rewind(fp);
        if (!fread_u64(fp, ENDIAN_BIG, val3)) {
          fioPerr();
          fprintf(stderr, " Error: fread_u64 ENDIAN_BIG is wrong\n");
          isOk=false;
        }
        if (val3!=0x0001020304050607) {
          fioPerr();
          fprintf(stderr, " Error: fread_u64 ENDIAN_BIG is wrong\n");
          isOk=false;
        }
        rewind(fp);
        vbuf.clear();
        vbuf = fileLoadBytes(fp);
        if (16!=vbuf.size()) {
          fioPerr();
          fprintf(stderr, " Error: fileLoadBytes has too few bytes\n");
          isOk=false;
        }
        /* fioPerr(); */
        /* fprintf(stderr, " Error: fileSaveBytes failed\n"); */
        /* isOk=false; */
        fclose(fp);
        fp=fileOpen(fname, "rb");
        if (fp) {
          // read 4 bytes
          vbuf.clear();
          vbuf = fileLoadBytes(fp, 4);
          if (4!=vbuf.size()) {
            fioPerr();
            fprintf(stderr, " Error: fileLoadBytes has wrong size (not 4)\n");
            isOk=false;
          }
          if (vbuf[0]!=0 || vbuf[1]!=1 || vbuf[2]!=2 || vbuf[3]!=3) {
            fioPerr();
            fprintf(stderr, " Error: fileLoadBytes result has wrong values\n");
            isOk=false;
          }
          // read 4 bytes again
          vbuf.clear();
          vbuf = fileLoadBytes(fp, 4);
          if (4!=vbuf.size()) {
            fioPerr();
            fprintf(stderr, " Error: fileLoadBytes has wrong size (not 4)\n");
            isOk=false;
          }
          if (vbuf[0]!=4 || vbuf[1]!=5 || vbuf[2]!=6 || vbuf[3]!=7) {
            fioPerr();
            fprintf(stderr, " Error: fileLoadBytes result has wrong values\n");
            isOk=false;
          }
          fclose(fp);
        }
      }

      fileDelete(fname);
    }
  }
  return isOk;
}
// SELFTEST
#endif

// HEADER
#endif
// EOF
