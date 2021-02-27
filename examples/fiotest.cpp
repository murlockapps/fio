// $VER: fiotest.cpp V1.0 (22.02.2021)

/* <COMPILE>
wflags="-W -Wall -Wextra -Wno-unused-parameter"
oflags="-Os -fno-exceptions -ffunction-sections -fdata-sections -fno-math-errno -fno-ident"
lflags="-Wl,--gc-sections"
g++ $wflags -pedantic $oflags -o fiotest fiotest.cpp -DSELFTEST
</COMPILE> */

/* <COMPILE_WIN>
g++ -W -Wall -Wextra -Wno-unused-parameter -pedantic -MD -Os -s -fno-exceptions -ffunction-sections -fdata-sections -fno-math-errno -fno-ident -fno-rtti -Wl,--gc-sections -static-libgcc -pie -o fiotest fiotest.cpp -DSELFTEST
strip fiotest.exe -s -R .comment -R .gnu.version -R .note
</COMPILE_WIN> */

#include "fio.h"

int main(int argc, char **argv) {
  bool isOk = fioSelftest();
  if (isOk) {
    fioPerr(1);
  }
  return 0;
}
// EOF
