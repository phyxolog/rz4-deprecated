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

#ifndef EJECT_H
#define EJECT_H

#include <string>
#include <fstream>

#include "boost/filesystem.hpp"

class Eject {
private:
  std::ifstream file;
  std::string file_name;
  unsigned int buffer_size;
  uintmax_t file_size;

public:
  Eject(std::string, unsigned int = 65536);
  ~Eject();

  bool extract(uintmax_t, uintmax_t, std::string);
  void close();
};

#endif