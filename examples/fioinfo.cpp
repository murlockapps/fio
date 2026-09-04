// $VER: fioinfo.cpp V1.1 (04.09.2026)

/* <COMPILE>
g++ -O2 -o fioinfo fioinfo.cpp
</COMPILE> */

/* <COMPILE_WIN>
g++ -O2 -o fioinfo fioinfo.cpp
</COMPILE_WIN> */

#include <iostream>
#include "fio.h"

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
// EOF
