#include "extractor.hpp"

namespace fs = boost::filesystem;

Extractor::Extractor(std::string file_name, uint buffer_size) : file_name(file_name), buffer_size(buffer_size) {
  file.open(file_name, std::fstream::binary);
  file_size = fs::file_size(file_name);

  if (file_size < buffer_size) {
    buffer_size = file_size;
  }
}

bool Extractor::extract(uintmax_t offset, uintmax_t size, std::string out_file_name) {  
  file.seekg(offset, std::ios::beg);

  if (size < buffer_size) {
    buffer_size = size;
  }

  if (size > file_size) {
    size = file_size;
  }

  uintmax_t read_bytes = 0;
  char *buffer = new char[buffer_size];
  std::ofstream out_file(out_file_name);

  if (!out_file.is_open()) {
    return false;
  }

  while (read_bytes < size) {
    if ((read_bytes + buffer_size) > size) {
      buffer_size = size - read_bytes;
      delete[] buffer;
      buffer = new char[buffer_size];
    }

    file.read(buffer, buffer_size);
    out_file.write(buffer, buffer_size);
    read_bytes += buffer_size;
  }

  delete[] buffer;
  out_file.close();

  return true;
}

Extractor::~Extractor() {
  file.close();
}

void Extractor::close() {
  if (file.is_open()) {
    file.close();
  }
}