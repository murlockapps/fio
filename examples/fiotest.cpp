// $VER: fiotest.cpp V1.2 (24.09.2026)
// Selftest for fio.h

/* <COMPILE>
wflags="-W -Wall -Wextra -Wno-unused-parameter"
oflags="-Os -fno-exceptions -ffunction-sections -fdata-sections -fno-math-errno -fno-ident"
lflags="-Wl,--gc-sections"
g++ $wflags -pedantic $oflags -o fiotest fiotest.cpp
</COMPILE> */

/* <COMPILE_WIN>
g++ -W -Wall -Wextra -Wno-unused-parameter -pedantic -MD -Os -s -fno-exceptions -ffunction-sections -fdata-sections -fno-math-errno -fno-ident -fno-rtti -Wl,--gc-sections -static-libgcc -pie -o fiotest fiotest.cpp
strip fiotest.exe -s -R .comment -R .gnu.version -R .note
</COMPILE_WIN> */

#include <inttypes.h>
#include <time.h>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#include "fio.h"

#pragma GCC diagnostic pop

void fioPerr(int m=0) {
  switch(m) {
  case 0:  fprintf(stderr, "fioSelftest:"); break;
  case 1: {
    fprintf(stdout, "fioSelftest [PASSED]");
#ifdef __linux__
    fprintf(stdout, " on Linux");
#elif defined(_WIN32)
    fprintf(stdout, " on Windows");
#endif
    time_t t = time(NULL);
    struct tm *datum = localtime(&t);

    fprintf(stdout, " (%02d.%02d.%04d)\n",
            datum->tm_mday,
            datum->tm_mon + 1,
            datum->tm_year + 1900);
  }
    break;
  case 2:  fprintf(stderr, "fioSelftest [FAILED]\n"); break;
  }
}

bool fioSelftest() {
  bool isOk = true;
  {
    // Test 1 - Check version information
    int exp_val = 1;
    if (FIO_VER != exp_val) {
      fioPerr();
      fprintf(stderr, " Error 1a: FIO_VER is not %d\n", exp_val);
      isOk = false;
    }
    exp_val = 7;
    if (FIO_REV != exp_val) {
      fioPerr();
      fprintf(stderr, " Error 1b: FIO_REV is not %d\n", exp_val);
      isOk = false;
    }
    const size_t SS=4;
    const char se[SS] = "1.7"; // expected value
    const char sv[SS] = FIO_VERSTR; // real value
    for (size_t i = 0; i < SS; i++) {
      if (sv[i] != se[i]) {
        fioPerr();
        fprintf(stderr, " Error 1c: FIO_VERSTR is not '%s'\n", se);
        isOk = false;
        break;
      }
    }
  }
  {
    // Test 2 - Check path separators and EOL
#ifdef __linux__
    const char ce = '/';
    const char *se = "\n";
#elif defined(_WIN32)
    const char ce = '\\';
    const char *se = "\r\n";
#endif
    if (ce != FIO_PATH_SEPARATOR) {
      fioPerr();
      fprintf(stderr, " Error 2a: FIO_PATH_SEPARATOR is not '%c'\n", ce);
      isOk = false;
    }
    const char *sv = FIO_EOL;
    if ( (se[0]!=sv[0]) || (se[1]!=sv[1])) {
      fioPerr();
#ifdef __linux__
      fprintf(stderr, " Error 2b: FIO_EOL is not '\\n'\n");
#elif defined(_WIN32)
      fprintf(stderr, " Error 2c: FIO_EOL is not '\\r\\n'\n");
#endif
      isOk = false;
    }
  }
  {
    // Test 3 - Check ENDIAN_LITTLE ENDIAN_BIG and FILEIOBUFSIZE
    if (false != ENDIAN_LITTLE) {
      fioPerr();
      fprintf(stderr, " Error 3a: ENDIAN_LITTLE is not 0\n");
      isOk = false;
    }
    if (true != ENDIAN_BIG) {
      fioPerr();
      fprintf(stderr, " Error 3b: ENDIAN_LITTLE is not 1\n");
      isOk = false;
    }
    int ie = 8192;
    if (ie != FILEIOBUFSIZE) {
      fioPerr();
      fprintf(stderr, " Error 3c: FILEIOBUFSIZE is not %d\n", ie);
      isOk = false;
    }
  }
  {
    // Test 4 - Check strSize
    if (0 != strSize(0) || 0 != strSize(NULL)) {
      fioPerr();
      fprintf(stderr, " Error 4a: strSize(0) is not 0\n");
      isOk = false;
    }
    if (0 != strSize("")) {
      fioPerr();
      fprintf(stderr, " Error 4b: strSize(\"\") is not 0\n");
      isOk = false;
    }
    if (0 != strSize("\0")) {
      fioPerr();
      fprintf(stderr, " Error 4c: strSize(\"\\0\") is not 0\n");
      isOk = false;
    }
    if (3 != strSize("foo")) {
      fioPerr();
      fprintf(stderr, " Error 4d: strSize(\"foo\") is not 3\n");
      isOk = false;
    }
    if (0 != strSize("\0foo")) {
      fioPerr();
      fprintf(stderr, " Error 4e: strSize(\"\\0foo\") is not 0\n");
      isOk = false;
    }
  }
  {
    // isBigEndian is not tested!
  }
  {
    // Test 5 - Check bswap_u16
    uint16_t s = 0x1122;
    if (0x2211 != bswap_u16(s)) {
      fioPerr();
      fprintf(stderr, " Error 5a: bswap_u16(0x1122) is incorrect\n");
      isOk = false;
    }
    if (s != bswap_u16(bswap_u16(s))) {
      fioPerr();
      fprintf(stderr, " Error 5b: bswap_u16(bswap_u16(0x1122)) is incorrect\n");
      isOk = false;
    }
    if (0 != bswap_u16(0)) {
      fioPerr();
      fprintf(stderr, " Error 5c: bswap_u16(0) is incorrect\n");
      isOk = false;
    }
  }
  {
    // Test 6 - Check bswap_u32
    uint32_t s = 0x11223344;
    if (0x44332211 != bswap_u32(s)) {
      fioPerr();
      fprintf(stderr, " Error 6a: bswap_u32(0x11223344) is incorrect\n");
      isOk = false;
    }
    if (0 != bswap_u32(0)) {
      fioPerr();
      fprintf(stderr, " Error 6b: bswap_u32(0) is incorrect\n");
      isOk = false;
    }
    if (0x78563412 != bswap_u32(0x12345678)) {
      fioPerr();
      fprintf(stderr, " Error 6c: bswap_u32(0x12345678) is incorrect\n");
      isOk = false;
    }
    if (0xFF000000 != bswap_u32(0x000000FF)) {
      fioPerr();
      fprintf(stderr, " Error 6d: bswap_u32(0x000000FF) is incorrect\n");
      isOk = false;
    }
    if (0xFF00AA55 != bswap_u32(0x55AA00FF)) {
      fioPerr();
      fprintf(stderr, " Error 6e: bswap_u32(0x55AA00FF) is incorrect\n");
      isOk = false;
    }
  }
  {
    // Test 7 - Check bswap_u64
    uint64_t n = 0x1122334455667788ULL;
    if (0x8877665544332211ULL != bswap_u64(n)) {
      fioPerr();
      fprintf(stderr, " Error 7a: bswap_u64(0x1122334455667788) is incorrect\n");
      isOk = false;
    }

    if (0 != bswap_u64(0)) {
      fioPerr();
      fprintf(stderr, " Error 7b: bswap_u64(0) is incorrect\n");
      isOk = false;
    }

    if (0xFF00000000000000ULL != bswap_u64(0x00000000000000FFULL)) {
      fioPerr();
      fprintf(stderr, " Error 7c: bswap_u64(0x00000000000000FF) is incorrect\n");
      isOk = false;
    }
  }
  {
    // Test 8 - Not existing file on
    // fileOpen fileExists fileReadable fileReadable fileSize fileModificationTime
    const char *fname = "tst.dat";
    FILE *fp = fileOpen(fname, "rb");
    if (fp) {
      fioPerr();
      fprintf(stderr, " Error 8a: fileOpen(\"%s\") falsely succeeded\n", fname);
      isOk = false;
    }
    if (fileExists(fname)) {
      fioPerr();
      fprintf(stderr, " Error 8b: fileExists(\"%s\") falsely succeeded\n", fname);
      isOk = false;
    }
    if (fileReadable(fname)) {
      fioPerr();
      fprintf(stderr, " Error 8c: fileReadable(\"%s\") falsely succeeded\n", fname);
      isOk = false;
    }
    int64_t len = fileSize(fname);
    if (len != -1) {
      fioPerr();
      fprintf(stderr, " Error 8d: fileSize(\"%s\") should return -1 for a non-existing file\n", fname);
      isOk = false;
    }
    int rc = fileType(fname);
    if (rc != -1) {
      fioPerr();
      fprintf(stderr, " Error 8e: fileType(\"%s\") should return -1 for a non-existing file\n", fname);
      isOk = false;
    }
    time_t mtime = fileModificationTime(fname);
    if (mtime != 0) {
      fioPerr();
      fprintf(stderr, " Error 8f: fileModificationTime(\"%s\") falsely succeeded\n", fname);
      isOk = false;
    }
  }
  {
    // Test 9 - Create a new file and write 8 bytes into it.
    // fileOpen fwrite_u64 fileClose
    const char *fname = "tst.dat";
    FILE *fp = fileOpen(fname, "wb");
    if (!fp) {
      fioPerr();
      fprintf(stderr, " Error 9a: fileOpen(\"%s\") failed to create file for writing.\n", fname);
      isOk = false;
    }
    uint64_t n = 0x1122334455667788ULL;
    if (!fwrite_u64(fp, false, n)) {
      fioPerr();
      fprintf(stderr, " Error 9b: fwrite_u64 failed to write into opened file \"%s\".\n", fname);
      isOk = false;
    }
    fileClose(fp);

    // Try to open this file for reading and close it again.
    fp = fileOpen(fname, "rb");
    if (!fp) {
      fioPerr();
      fprintf(stderr, " Error 9c: fileOpen(\"%s\") failed to open file for reading.\n", fname);
      isOk = false;
    }
    fileClose(fp);
  }
  {
    // Test 10 - Tests with existing file.
    // fileSize fileReadable fileExists fileType fileModificationTime
    const char *fname = "tst.dat";
    int64_t len = fileSize(fname);
    if (len != 8) {
      fioPerr();
      fprintf(stderr, " Error 10a: fileSize(\"%s\") provides incorrect data (is %d but shout be %d)\n", fname, (int)len, 8);
      isOk = false;
    }
    if (!fileReadable(fname)) {
      fioPerr();
      fprintf(stderr, " Error 10b: fileReadable(\"%s\") failed\n", fname);
      isOk = false;
    }
    if (!fileExists(fname)) {
      fioPerr();
      fprintf(stderr, " Error 10c: fileExists(\"%s\") failed\n", fname);
      isOk = false;
    }
    if (0 != fileType(fname)) {
      fioPerr();
      fprintf(stderr, " Error 10d: fileType(\"%s\") failed, should be a file\n", fname);
      isOk = false;
    }
    if (1 != fileType(".")) {
      fioPerr();
      fprintf(stderr, " Error 10e: fileType(\".\") failed, should be a directory\n");
      isOk = false;
    }
    time_t rawtime;
    time(&rawtime);
    time_t mtime = fileModificationTime(fname);
    if (mtime == 0) {
      fioPerr();
      fprintf(stderr, " Error 10f: fileModificationTime(\"%s\") failed\n", fname);
      isOk = false;
    }
    time_t tdiff=abs(mtime-rawtime);
    if (tdiff!=0) {
      fioPerr();
      fprintf(stderr, " Error 10g: fileModificationTime(\"%s\") is inaccurate\n", fname);
      isOk=false;
    }
  }
  {
    // Test 11 -  Further tests with existing file.
    // fileOpen filePosition
    const char *fname = "tst.dat";
    FILE *fp = fileOpen(fname, "rb");
    if (!fp) {
      fioPerr();
      fprintf(stderr, " Error 11a: fileOpen(\"%s\") failed to open file for reading.\n", fname);
      isOk = false;
    }
    int64_t pos = filePosition(fp);
    if (pos != 0) {
      fioPerr();
      fprintf(stderr, " Error 11b: filePosition failed to open file for reading (should be 0 but is %d).\n", (int)pos);
      isOk = false;
    }
    fileClose(fp);
  }
  {
    // Test 12 - Delete file and try to reopen
    const char *fname = "tst.dat";
    fileDelete(fname);
    FILE *fp = fileOpen(fname, "rb");
    if (fp) {
      fioPerr();
      fprintf(stderr, " Error 12a: fileDelete(\"%s\") failed because the file still exists\n", fname);
      isOk = false;
      fileClose(fp);
    }
  }

  return isOk;
}

int main(int argc, char **argv) {
  bool isOk = fioSelftest();
  if (isOk) {
    fioPerr(1);
  }
  return 0;
}
// EOF
