#include <iostream>
#include <fstream>
#include <string>

void usage() {
  std::cout << "./bytecounter <filename>\n";
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    usage();
    return -1;
  }

  // open specified file in binary mode
  auto fname = argv[1];
  std::ifstream in_file(fname, std::ios::binary);

  unsigned long bytes = 0;
  char buf[1];
  while (in_file.read(buf, 1)) {
    ++bytes;
  }

}