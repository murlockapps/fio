// $VER: fioinfo.cpp V1.0 (27.02.2021)

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

  std::cout << "2. The PATH_SEPARATOR string sequence is ";
  std::cout << " '" << PATH_SEPARATOR << "'" << std::endl;
  return 0;
}
// EOF
