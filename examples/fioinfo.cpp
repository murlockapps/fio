// $VER: fioinfo.cpp V1.0 (27.02.2021)
#include <iostream>
#include "fio.h"

/* <COMPILE>
g++ -O2 -o fioinfo fio.cpp
</COMPILE> */

/* <COMPILE_WIN>
g++ -O2 -o example1 example1.cpp
</COMPILE_WIN> */

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
