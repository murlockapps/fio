# About

fio.h V1.6 (03.09.2026) Public Domain (PD)

Portable file functions for basic input and output (Linux and Windows)
fio is a small library for basic file operations.
It comes as an STB-style single-file library with no external dependencies.

## Features
* Support for large files (64bit)
* All strings support UTF-8 encoding
* Same API on all platforms (therefore also same code)
* System-specific abstractions for path separator and EOL
* Byte-order-specific read and write functions
* Single-header library (no extra compitaion is necessary)
* Standalone, no extra libraries needed
  
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

   std::cout << "2. The path separator string sequence is ";
   std::cout << " '" << FIO_PATH_SEPARATOR << "'" << std::endl;
   return 0;
}
```

### Write 'hello world'
```c++
#include "fio.h"
int main(int argc, char **argv) {
  FILE *fp = fileOpen("fiohello.txt", "wb");
  if (fp) {
    std::vector<uint8_t> v;
    const char *cstr = "Hello World!\n";
    for (size_t i = 0; cstr[i] != '\0'; i++) {
      v.push_back(cstr[i]);
    }
    fwrite_bytes(fp, v, v.size());
    fileClose(fp);
  }
  return 0;
}
```

## Contact
Feel free to contact me if there are any problems or further questions.  
Please send fio bug reports to 'info@murlock.de'
