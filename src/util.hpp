#ifndef __UTIL_H
#define __UTIL_H

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>

#include "boost/format.hpp"
#include "boost/filesystem.hpp"

// struct of found signatures
// in `data` field save header or null
typedef struct stream_info {
  std::string file_type;
  std::string ext;
  std::string short_type;
  size_t file_size;
  size_t offset;
  void *data;
} stream_info;

std::string humnsize(uintmax_t);
std::string tmpfile(const std::string&);
std::string prettytime(long long);
uintmax_t memtoll(std::string);
int charmatch(const char*, uint, char, uint = 0);

#endif