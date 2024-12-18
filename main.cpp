#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <regex>
#include <optional>
#include <limits>

const std::string ENCODED_SUFFIX = ".encoded";

void usage()
{
  std::cout << "./kompress <mode encode/decode (-e/-d)> <filename>\n";
}

void encode_RLE(std::string fname)
{

  std::ifstream in_file(fname, std::ios::binary);

  std::ofstream out_file(fname + ".encoded", std::ios::binary);

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

  std::ifstream encoded_file(fname, std::ios::binary);
  std::string output_filename = fname.substr(fname.find_last_of("/") + 1);

  if (output_filename.ends_with(ENCODED_SUFFIX))
  {
    output_filename = output_filename.substr(0, output_filename.size() - ENCODED_SUFFIX.size());
  }

  std::ofstream decoded_file(output_filename, std::ios::binary);

  if (!decoded_file.is_open())
  {
    std::cerr << "Error: Could not create output file: " << fname + ".decoded" << "\n";
    return;
  }

  char buf[2];
  while (encoded_file.read(buf, sizeof(buf)))
  {
    unsigned char count = static_cast<unsigned char>(buf[0]);
    char character = buf[1];
    for (int i = 0; i < count; ++i)
    {
      decoded_file.write(&character, 1);
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