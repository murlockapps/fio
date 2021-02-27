// $VER: fiohello.cpp V1.0 (27.02.2021)

/* <COMPILE>
g++ -O2 -o fiohello fiohello.cpp
</COMPILE> */

/* <COMPILE_WIN>
g++ -O2 -o fiohello fiohello.cpp
</COMPILE_WIN> */

#include "fio.h"

int main(int argc, char **argv) {
  FILE *fp=fileOpen("/tmp/hello.txt", "wb");
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
// EOF
