#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include <algorithm>
#include <functional>

#define NO_OF_CHARS 256

class Scanner {
private:
  std::ifstream file;
  std::string file_name;
  unsigned int buffer_size;
  unsigned long long int file_size;
  unsigned long long int current_offset;

public:
  Scanner(std::string, unsigned int);
  bool scan();

  void riff_wave_scanner(const char*, unsigned long long);
};

#endif