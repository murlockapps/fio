==========================================
 ReadMe fio - A portable file I/O library
==========================================

Overview:
 fio.h V1.6 (03.09.2026)
 Copyright (C) 2026 Michael Sobol info@murlock.de - Public Domain (PD)

 Portable file functions for basic input and output (Linux and Windows)
 fio is a small library for basic file operations.
 It comes as an STB-style single-file library with no external dependencies.

Features:
 +Support for large files (64-bit)
 +All strings support UTF-8 encoding
 +Same API on all platforms (therefore also same code)
 +System-specific abstractions for path separator and EOL
 +Byte-order-specific read and write functions
 +Single-header library (no extra compitaion is necessary)
 +Standalone, no extra libraries needed

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

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

 For more information, please refer to <http://unlicense.org>

Passed tests:
 openSUSE Leap 15.2           -> 22.05.2023
 Devuan GNU/Linux 3 (beowulf) -> 22.05.2023
 Windows 10 Pro               -> 22.05.2023
 Windows 11 Pro               -> 03.09.2026

Compatible compilers:
 g++ 8.3.0
 TDM-GCC 9.2.0
 TDM-GCC 10.3.0

Version history:
 V1.6 (03.09.2026):
  Return value was wrong in fread_bytes.
  The self-test was outsourced into fiotest.cpp.
  Unnecessary macros removed and portable additional functions added
  such as filePosition and fileSeek.
 V1.5 (11.09.2025):
  Rename functions fileLoadBytes fileSaveBytes into fread_bytes fwrite_bytes
 V1.4 (23.08.2025):
  New functions fread_flt, fread_dbl, fwrite_flt and fwrite_dbl.
 V1.3 (22.05.2023):
  New functions fread_u8 and fwrite_u8.
  Changed behaviour if file pointer is invalid.
 V1.2 (29.06.2021):
  Compiler bugfix for windows
  WINVER undef
 V1.1 (15.03.2021):
  The functions fread_u16, fread_u32 and fread_u64 have been changed.
  They return true on success and false on fail now.
  The documentation was revised.

Feel free to contact me if there are any problems or further questions.
Please send fio bug reports to info@murlock.de

-------------
Short Manual:
-------------

Definitions:
 FIO_VER     = 1      -> Version number
 FIO_REV     = 6      -> Revision number
 FIO_VERSTR  = "1.6"  -> Version string

 FIO_PATH_SEPARATOR -> System specific path seperator
 FIO_EOL            -> System specific line ending sequence

 ENDIAN_LITTLE = 0
 ENDIAN_BIG    = 1
 FILEIOBUFSIZE = 8192

Functions:
 strSize : size_t strSize(const char *s)
  Returns size in bytes from string 's'

 isBigEndian : bool isBigEndian()
  Returns true on big-endian systems and false otherwise.

 bswap_16 : int16_t bswap_16(int16_t v)
  Swaps the byte order between little-endian and big-endian (int16_t)
 
 bswap_u16 : uint16_t bswap_u16(uint16_t v)
  Swaps the byte order between little-endian and big-endian (uint16_t)
 
 bswap_32 : int32_t bswap_32(int32_t v)
  Swaps the byte order between little-endian and big-endian (int32_t)

 bswap_u32 : uint32_t bswap_u32(uint32_t v)
  Swaps the byte order between little-endian and big-endian (uint32_t)
 
 bswap_u64 : uint64_t bswap_u64(uint64_t v)
  Swaps the byte order between little-endian and big-endian (uint64_t)

 fread_u8 : bool fread_u8(FILE *fp, uint8_t &rv)
  Reads a single byte from the file pointer 'fp'.
  Stores the byte read from the file in the variable referenced by 'rv'.
  Returns true on success and false otherwise.

 fread_u16 : bool fread_u16(FILE *fp, bool bBigEndian, uint16_t &rv)
  Reads an unsigned short (2 bytes) from the file pointer 'fp'.
  Stores the value read from the file in the variable referenced by 'rv'.
  If 'bBigEndian' is true, the byte order is interpreted as big-endian.
  Returns true on success and false otherwise.

 fread_u32 : bool fread_u32(FILE *fp, bool bBigEndian, uint32_t &rv)
  Reads an unsigned int (4 bytes) from the file pointer 'fp'.
  Stores the value read from the file in the variable referenced by 'rv'.
  If 'bBigEndian' is true, the byte order is interpreted as big-endian.
  Returns true on success and false otherwise.

 fread_u64 : bool fread_u64(FILE *fp, bool bBigEndian, uint64_t &rv)
  Reads an uint64_t (8 bytes) from the file pointer 'fp'.
  Stores the value read from the file in the variable referenced by 'rv'.
  If 'bBigEndian' is true, the byte order is interpreted as big-endian.
  Returns true on success and false otherwise.

 fread_flt : bool fread_flt(FILE *fp, bool bBigEndian, float &rv)
  Reads a float value (4 bytes) from the file pointer 'fp'.
  Stores the value read from the file in the variable referenced by 'rv'.
  If 'bBigEndian' is true, the byte order is interpreted as big-endian.
  Returns true on success and false otherwise.

 fread_dbl : bool fread_dbl(FILE *fp, bool bBigEndian, double &rv)
  Reads a double value (8 bytes) from the file pointer 'fp'.
  Stores the value read from the file in the variable referenced by 'rv'.
  If 'bBigEndian' is true, the byte order is interpreted as big-endian.
  Returns true on success and false otherwise.

 fread_bytes : fread_bytes(FILE *fp, std::vector &rv, int64_t len = 0)
  Loads len bytes from the current file position into a vector of bytes.
  If len is zero, then all remaining bytes are loaded up to the end of the file.
  Returns true if all requested bytes were successfully read, otherwise false.

 fwrite_u8 : bool fwrite_u8(FILE *fp, uint8_t v)
  Writes a single byte value 'v' to the file pointer 'fp'.
  Returns true on success and false otherwise.

 fwrite_u16 : bool fwrite_u16(FILE *fp, bool bBigEndian, uint16_t v)
  Writes an unsigned short (2 bytes) value 'v' to file pointer 'fp'.
  If 'bBigEndian' is true, the value is written in big-endian byte order.
  Returns true on success and false otherwise.

 fwrite_u32 : bool fwrite_u32(FILE *fp, bool bBigEndian, uint32_t v)
  Writes an unsigned int (4 bytes) value 'v' to file pointer 'fp'.
  If 'bBigEndian' is true, the value is written in big-endian byte order.
  Returns true on success and false otherwise.

 fwrite_u64 : bool fwrite_u64(FILE *fp, bool bBigEndian, uint64_t v) 
  Writes an uint64_t (8 bytes) value 'v' to file pointer 'fp'.
  If 'bBigEndian' is true, the value is written in big-endian byte order.
  Returns true on success and false otherwise.

 fwrite_flt : bool fwrite_flt(FILE *fp, bool bBigEndian, float v)
  Writes a float (4 bytes) value 'v' to file pointer 'fp'.
  If 'bBigEndian' is true, the value is written in big-endian byte order.
  Returns true on success and false otherwise.

 fwrite_dbl : bool fwrite_dbl(FILE *fp, bool bBigEndian, double v)
  Writes a double (8 bytes) value 'v' to file pointer 'fp'.
  If 'bBigEndian' is true, the value is written in big-endian byte order. Returns true on success and false otherwise.

 fwrite_bytes : bool fwrite_bytes(FILE *fp, const std::vector &v, int64_t len=0)
  Saves 'len' bytes from the given vector v into file fp.
  If 'len' is zero, then the whole vector is written to the file.
  Returns true if all requested bytes were successfully written, otherwise false.

 fileOpen : FILE* fileOpen(const char *fullpath, const char *mode)
  Opens a file 'fullpath' with given mode 'mode'.
  Extra large files can be opened.

  The access modes can be:
   'r'  Open a file for reading. The file must exist.
   'w'  Create an empty file for writing
        If the file already exists, it will be overwritten.
   'a'  Open a file for appending.
        Data is always written at the end of the file, expanding the file.
   'r+' Open a file for both reading and writing.
        The file must exist.
   'w+' Create an empty file for both reading and writing
        If a file with the same name already exists, its contents are discarded and the file is treated as a new, empty file.
   'a+' Open a file for both reading and writing.
        All output operations write data at the end of the file.
        Repositioning operations (fseek, fsetpos, rewind) affect the next input operation,
        but output operations move the position back to the end of the file.
        The file is created if it does not exist.

  With the mode specifiers above the file is open as a text file.
  In order to open a file as a binary file, a 'b' character has to be included in the mode string.
  This additional 'b' character can either be appended at the end of the string
  (thus making the following compound modes: 'rb', 'wb', 'ab', 'r+b', 'w+b', 'a+b')
  or be inserted between the letter and the '+' sign for the mixed modes ('rb+', 'wb+', 'ab+').

 fileClose : int fileClose(FILE *fp)
  Closes a file 'fp' that was previously opened with fileOpen.

 fileSize  int64_t fileSize(const char *fullpath)
  Returns the size of a given file 'fullpath' in bytes or -1 on errors.

 fileSize : int64_t fileSize(FILE *fp)
  Returns the size of a given file descriptor 'fp' in bytes or -1 on errors.

 filePosition : int64_t filePosition(FILE *fp)
  Returns the current file position in fp or -1 on errors.

 fileSeek : int fileSeek(FILE *fp, int64_t offset, int origin)
  Sets the file position in fp according to offset and origin.
  Returns -1 on errors.

 fileReadable - bool fileReadable(const char *fullpath)
  Returns true if file 'fullpath' or directory is readable.

 fileExists : bool fileExists(const char *fullpath)
  Returns true if given file 'fullpath' exists, otherwise false.
  Files can exist but are not readable.

 fileType : int fileType(const char *fullpath)
 Returns the type of a file (-1=error, 0=file, 1=directory, 2=symlink)

 fileModificationTime : time_t fileModificationTime(const char *fullpath)
  Returns the modification time of a file 'fullpath'

 fileDelete : bool fileDelete(const char *fullpath)
  Deletes a file 'fullpath'.

 fileRename : bool fileRename(const char *oldpath, const char *newpath)
  Renames a file from 'oldpath' into 'newpath'

 fileCreateDirectory : bool fileCreateDirectory(const char *fullpath, int mode=0777)
  Creates a directory.
  The specific permissions are ignored on Windows.

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
 12:   std::cout << "2. The path separator string sequence is ";
 13:   std::cout << " '" << FIO_PATH_SEPARATOR << "'" << std::endl;
 14:   return 0;
 15: }

Example 2 - write "hello world!\" to file "/tmp/hw.txt":
 01: #include "fio.h"
 02: int main(int argc, char **argv) {
 03:   FILE *fp = fileOpen("/tmp/hw.txt", "wb");
 04:   if (fp) {
 05:     std::vector<uint8_t> v;
 06:     const char *cstr = "Hello World!\n";
 07:     for (size_t i=0; cstr[i]!='\0'; i++) {
 08:       v.push_back(cstr[i]);
 09:     }
 10:     fwrite_bytes(fp, v, v.size());
 11:     fileClose(fp);
 12:   }
 13:   return 0;
 14: }

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
 Please send fio bug reports to 'info@murlock.de'.
