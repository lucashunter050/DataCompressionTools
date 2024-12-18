#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <regex>
#include <optional>
#include <limits>

void usage()
{
  std::cout << "./kompress <filename>\n";
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    usage();
    return -1;
  }

  // open specified file in binary mode
  auto fname = std::string(argv[1]);

  // std::regex extension_pattern("\\w*.\w");

  std::ifstream in_file(fname, std::ios::binary);

  std::ofstream out_file("rLEcompressedtest", std::ios::binary);
  

  unsigned long bytes = 0;
  std::optional<char> first_unique;

  char buf;
  unsigned char curr_count = 0;

  while (in_file.read(&buf, 1))
  {
    std::cout << "read " << buf << "\n";
    if (first_unique.has_value())
    {
      std::cout << "option has a value\n";
      if (first_unique.value() == buf)
      {
        if (curr_count < std::numeric_limits<unsigned char>::digits)
        {
          ++curr_count;
        }
        else
        {
          out_file.write(reinterpret_cast<const char *>(&curr_count), sizeof(curr_count));
          out_file.write(reinterpret_cast<const char *>(&first_unique.value()), 1);
          curr_count = 1;
        }
      }
      else
      {
        std::cout << "123\n";
        out_file.write(reinterpret_cast<const char *>(&curr_count), sizeof(curr_count));
        std::cout << "4567\n";
        out_file.write(reinterpret_cast<const char *>(&first_unique.value()), 1);
        std::cout << "you make me feel like 11\n";
        first_unique = buf;
        curr_count = 1;
      }
    }
    else
    {
      std::cout << "first run of program\n";
      // first byte in the file
      first_unique = buf;
      curr_count = 1;
    }
  }

  std::cout << bytes << "\n";
}