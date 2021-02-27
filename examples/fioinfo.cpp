// $VER: example1.cpp V1.0 (27.02.2021)
// Copyright (C) 2021 Michael Sobol info@murlock.de
// Insert a short description here.
#include <iostream>
#include "fio.h"

/* <COMPILE>
g++ -O2 -o example1 example1.cpp
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
