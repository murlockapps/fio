// $VER: fiohello.cpp V1.1 (04.09.2026)

/* <COMPILE>
g++ -O2 -o fiohello fiohello.cpp
</COMPILE> */

/* <COMPILE_WIN>
g++ -O2 -o fiohello fiohello.cpp
</COMPILE_WIN> */

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
// EOF
