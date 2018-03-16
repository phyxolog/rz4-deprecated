#ifndef EJECT_H
#define EJECT_H

#include <string>
#include <fstream>

#include "boost/filesystem.hpp"

class Eject {
private:
  std::ifstream file;
  std::string file_name;
  uint buffer_size;
  uintmax_t file_size;

public:
  Eject(std::string, uint = 65536);
  ~Eject();

  bool extract(uintmax_t, uintmax_t, std::string);
  void close();
};

#endif