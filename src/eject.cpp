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

#include "eject.hpp"

namespace fs = boost::filesystem;

Eject::Eject(std::string file_name, unsigned int buffer_size) : file_name(file_name), buffer_size(buffer_size) {
  file.open(file_name, std::fstream::binary);
  file_size = fs::file_size(file_name);

  if (file_size < buffer_size) {
    buffer_size = file_size;
  }
}

bool Eject::extract(uintmax_t offset, uintmax_t size, std::string out_file_name) {  
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

void Eject::close() {
  if (file.is_open()) {
    file.close();
  }
}

Eject::~Eject() {
  file.close();
}