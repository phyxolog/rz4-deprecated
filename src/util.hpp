/*
 * Copyright (C) 2018 Yura Zhivaga <yzhivaga@gmail.com>
 * 
 * This file is part of rz4.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
std::string prettytime(uintmax_t);
uintmax_t memtoll(std::string);
int charmatch(const char*, uint, char, uint = 0);

#endif