#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <string>
#include <fstream>

class Extractor {
private:
  std::ifstream file;
  std::string file_name;
  unsigned int buffer_size;
  unsigned long long file_size;

public:
  Extractor(std::string, unsigned int = 65536);
  ~Extractor();

  bool extract(unsigned long long, unsigned long long, std::string);
};

#endif