#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

class Scanner {
private:
  std::ifstream file;
  std::string file_name;
  int buffer_size;
  unsigned long long int file_size;
  unsigned long long int current_offset;

public:
  Scanner(std::string, int);
  bool scan();
};

#endif