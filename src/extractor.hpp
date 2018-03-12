#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <string>
#include <fstream>

#include "boost/filesystem.hpp"

class Extractor {
private:
  std::ifstream file;
  std::string file_name;
  uint buffer_size;
  uintmax_t file_size;

public:
  Extractor(std::string, uint = 65536);
  ~Extractor();

  bool extract(uintmax_t, uintmax_t, std::string);
  void close();
};

#endif