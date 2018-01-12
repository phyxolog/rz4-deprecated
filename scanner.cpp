#include "scanner.hpp"

Scanner::Scanner(std::string file_name, int buffer_size = 65536) : file_name(file_name), buffer_size(buffer_size) {
  file.open(file_name, std::fstream::binary);
  file.seekg(0, std::ios::end);
  file_size = file.tellg();
  file.seekg(0, std::ios::beg);
}