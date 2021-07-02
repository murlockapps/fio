# About

fio.h V1.2 (29.06.2021) Public Domain (PD)

fio is a small library for very basic file functions.
It comes as an STB-style single-file library, with no external dependencies.

## Features
* support for large files (64bit)
* all strings can contain utf8 encoding
* same functions on all platforms (therefore also same code)
* fseeko64 ftello64 fopen64 fstat64 stat64 ststat64 on all systems
* system specific abstractions for PATH_SEPARATOR and EOL
* byteorder specific read and write functions
* single header library (no extra compiling is necessary)
* standalone, no extra libraries needed

## Examples

### Some system information
```c++
#include "fio.h"
#include <iostream>
int main(int argc, char **argv) {
  std::cout << "1. Your system is a ";
  if (isBigEndian()) {
    std::cout << "'BIG ENDIAN' ";
   } else {
     std::cout << "'LITTLE ENDIAN' ";
   }
   std::cout << "computer" << std::endl;

   std::cout << "2. The PATH_SEPARATOR string sequence is ";
   std::cout << " '" << PATH_SEPARATOR << "'" << std::endl;
   return 0;
}
```

### Write 'hello world'
```c++
#include "fio.h"
int main(int argc, char **argv) {
  FILE *fp=fileOpen("/tmp/hw.txt", "wb");
  if (fp) {
    std::vector<uint8_t> v;
    const char *cstr="Hello World!\n";
    for (size_t i=0; cstr[i]!='\0'; i++) {
      v.push_back(cstr[i]);
    }
    fileSaveBytes(fp, v, v.size());
    fileClose(fp);
  }
  return 0;
}
```

### Run selftest
```c++
#include "fio.h"
int main(int argc, char **argv) {
  bool isOk = fioSelftest();
  if (isOk) {
    fioPerr(1);
  }
  return 0;
}
```

#### Compile selftest (linux) with:
```bash
g++ -Wall -pedantic -Os -s -o selftest selftest.cpp -DSELFTEST
```


## Contact
Feel free to contact me if there are any problems or further questions.  
Please send fio bug reports to info@murlock.de
