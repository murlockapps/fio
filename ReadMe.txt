ReadMe fio - portable file input output library
-----------------------------------------------
Overview:
 fio.h V1.0 (22.02.2021)
 Copyright (C) 2021 Michael Sobol info@murlock.de
 Public Domain (PD)

 Portable file functions for basic input and output (linux and windows)
 fio is a small library for very basic file functions.
 It comes as an STB-style single-file library, with no external dependencies.

Features:
 +support for large files (64bit)
 +all strings can contain utf8 encoding
 +same functions on all platforms (therefore also same code)
 +fseeko64 ftello64 fopen64 fstat64 stat64 ststat64 on all systems
 +system specific abstractions for PATH_SEPARATOR and EOL
 +byteorder specific read and write functions
 +single header library (no extra compiling is necessary)
 +standalone, no extra libraries needed

License:
 The fio software is Public Domain (PD).
 This is free and unencumbered software released into the public domain.

 Anyone is free to copy, modify, publish, use, compile, sell, or
 distribute this software, either in source code form or as a compiled
 binary, for any purpose, commercial or non-commercial, and by any means.

 In jurisdictions that recognize copyright laws, the author or authors
 of this software dedicate any and all copyright interest in the
 software to the public domain. We make this dedication for the benefit
 of the public at large and to the detriment of our heirs and
 successors. We intend this dedication to be an overt act of
 relinquishment in perpetuity of all present and future rights to this
 software under copyright law.

 THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

 For more information, please refer to <http://unlicense.org>

Passed tests:
 openSUSE Leap 15.2           -> 25.02.2021
 Devuan GNU/Linux 3 (beowulf) -> 25.02.2021
 Windows 10 Pro               -> 25.02.2021

Compatible compilers:
 g++ 8.3.0
 TDM-GCC 9.2.0

-------------
Short Manual:
-------------

Definitions:
 FIO_VER     -> Version number
 FIO_REV     -> Revision number
 FIO_VERSTR  -> Version string

 PATH_SEPARATOR -> System specific path seperator
 EOL            -> System specific line ending sequence

 ENDIAN_LITTLE = 0
 ENDIAN_BIG    = 1
 FILEIOBUFSIZE = 8192

Functional unifications:
 fseeko64, fopen64, fstat64, ftello64 -> functions for large files

Functions:
 strSize -> Returns string length in bytes
  size_t strSize(const char *s);

 isBigEndian -> Returns true on big endian systems otherwise false
  bool isBigEndian(void);

 bswap_16 : swap endianess for 16 bit signed integer
  int16_t  bswap_16(int16_t v);

 bswap_u16 : swap endianess for 16 bit unsigned integer
  uint16_t bswap_u16(uint16_t v);

 bswap_32 : swap endianess for 32 bit signed integer
  int32_t  bswap_32(int32_t v);

 bswap_32 : swap endianess for 32 bit unsigned integer
  uint32_t bswap_u32(uint32_t v);

 bswap_64 : swap endianess for 64 bit signed integer
  int64_t  bswap_64(int64_t v);

 bswap_64 : swap endianess for 64 bit unsigned integer
  uint64_t bswap_u64(uint64_t v);

 fread_u16 : read unsigned 16 bit integer from given file fp in required endianess
  uint16_t fread_u16(FILE *fp, bool bBigEndian);

 fread_u32 : read unsigned 32 bit integer from given file fp in required endianess
  uint32_t fread_u32(FILE *fp, bool bBigEndian);

 fread_u64 : read unsigned 64 bit integer from given file fp in required endianess
  uint64_t fread_u64(FILE *fp, bool bBigEndian);

 fwrite_u16 : write unsigned 16 bit integer into given file fp in required endianess
  bool fwrite_u16(FILE *fp, bool bBigEndian, uint16_t v);

 fwrite_u32 : write unsigned 32 bit integer into given file fp in required endianess
  bool fwrite_u32(FILE *fp, bool bBigEndian, uint32_t v);

 fwrite_u64 : write unsigned 64 bit integer into given file fp in required endianess
  bool fwrite_u64(FILE *fp, bool bBigEndian, uint64_t v);

 fileLoadBytes : load len bytes from given file fp and return result vector
 If len is zero, then the whole file is loaded up to the end of the file.
  std::vector<uint8_t> fileLoadBytes(FILE *fp, int64_t len=0);

 fileSaveBytes : save len bytes from given vector v into file fp
 If len is zero, then write the whole vector into the file fp.
 Returns true if successfull, otherwise false.
  bool fileSaveBytes(FILE *fp, std::vector<uint8_t> &v, int64_t len=0);

 fileOpen : open file fullpath in given access mode
  FILE* fileOpen(const char *fullpath, const char *mode);

 fileClose : close file
  int fileClose(FILE *fp);

 fileSize : return file size for given file fullpath
  int64_t fileSize(const char *fullpath);

 fileSize : return file size for given file fp
  int64_t fileSize(FILE *fp);

 fileReadable : return true if file is readable otherwise false
  bool fileReadable(const char *fullpath);

 fileExists : return true if file exists otherwise false
  bool fileExists(const char *fullpath);

 fileType : return file type -1=error, 0=file, 1=directory, 2=symlink (linux only)
  int fileType(const char *fullpath);

 fileModificationTime -> return file modification time from file fullpath
  time_t fileModificationTime(const char *fullpath);

 fileDelete -> delete given file fullpath (unlink)
  bool fileDelete(const char *fullpath);

---------
Examples:
---------

Example 1 - give some information about your system:
 01: #include "fio.h"
 02: #include <iostream>
 03: int main(int argc, char **argv) {
 04:   std::cout << "1. Your system is a ";
 05:   if (isBigEndian()) {
 06:     std::cout << "'BIG ENDIAN' ";
 07:   } else {
 08:     std::cout << "'LITTLE ENDIAN' ";
 09:   }
 10:   std::cout << "computer" << std::endl;
 11:
 12:   std::cout << "2. The PATH_SEPARATOR string sequence is ";
 13:   std::cout << " '" << PATH_SEPARATOR << "'" << std::endl;
 14:   return 0;
 15: }

Example 2 - write "hello world!\" to file "/tmp/hw.txt":
 01: #include "fio.h"
 02: int main(int argc, char **argv) {
 03:   FILE *fp=fileOpen("/tmp/hw.txt", "wb");
 04:   if (fp) {
 05:     std::vector<uint8_t> v;
 06:     const char *cstr="Hello World!\n";
 07:     for (size_t i=0; cstr[i]!='\0'; i++) {
 08:       v.push_back(cstr[i]);
 09:     }
 10:     fileSaveBytes(fp, v, v.size());
 11:     fileClose(fp);
 12:   }
 13:   return 0;
 14: }

Example 3 - run selftest:
 01: #include "fio.h"
 02: int main(int argc, char **argv) {
 03:   bool isOk = fioSelftest();
 04:   if (isOk) {
 05:     fioPerr(1);
 06:   }
 07:   return 0;
 08: }

Compile selftest (linux) with:
 g++ -Wall -pedantic -Os -s -o fiotest fiotest.cpp -DSELFTEST

------
Links:
------
 http://www.murlock.de/
 https://github.com/murlockapps/fio
 https://guides.github.com/features/mastering-markdown/

--------
Contact:
--------
 Feel free to contact me if there are any problems or further questions.
 Please send fio bug reports to info@murlock.de
