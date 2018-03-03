#include "extractor.hpp"

Extractor::Extractor(std::string file_name, unsigned int buffer_size) : file_name(file_name), buffer_size(buffer_size) {
  file.open(file_name, std::fstream::binary);
  file.seekg(0, std::ios::end);
  file_size = file.tellg();
  file.seekg(0, std::ios::beg);

  if (file_size < buffer_size) {
    buffer_size = file_size;
  }
}

Extractor::~Extractor() {
  file.close();
}