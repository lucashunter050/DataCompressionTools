#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <regex>
#include <optional>
#include <limits>

void usage()
{
  std::cout << "./kompress <mode encode/decode (-e/-d)> <filename>\n";
}

void encode_RLE(std::string fname)
{
  std::cout << "encoding " << fname << "\n";
  // std::regex extension_pattern("\\w*.\w");

  std::ifstream in_file(fname, std::ios::binary);

  std::ofstream out_file("rLEcompressedtest", std::ios::binary);

  std::optional<char> first_unique;

  char buf;
  unsigned char curr_count = 0;

  while (in_file.read(&buf, 1))
  {
    if (first_unique.has_value())
    {
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
        out_file.write(reinterpret_cast<const char *>(&curr_count), sizeof(curr_count));
        out_file.write(reinterpret_cast<const char *>(&first_unique.value()), 1);
        first_unique = buf;
        curr_count = 1;
      }
    }
    else
    {
      // first byte in the file
      first_unique = buf;
      curr_count = 1;
    }
  }

  out_file.write(reinterpret_cast<const char *>(&curr_count), sizeof(curr_count));
  out_file.write(reinterpret_cast<const char *>(&first_unique.value()), 1);

}

void decodeRLE(std::string fname)
{
  std::cout << "decoding " << fname << "\n";
  std::ifstream encoded_file(fname);
  std::ofstream decoded_file("decoded_" + fname + ".decoded");

  char buf[2];
  while (encoded_file.read(buf, sizeof(buf)))
  {
    unsigned char count = static_cast<unsigned char>(buf[0]);
    char character = buf[1];
    std::cout << "read buffer\n";
    std::cout << "writing " << character << " " << static_cast<int>(count) << " times\n";
    for (int i = 0; i < count; ++i)
    {
      decoded_file.write(&buf[1], 1);
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    usage();
    return -1;
  }

  auto mode = std::string(argv[1]);
  auto fname = std::string(argv[2]);

  if (mode == "-e")
  {
    encode_RLE(fname);
  }
  else if (mode == "-d")
  {
    decodeRLE(fname);
  }
}