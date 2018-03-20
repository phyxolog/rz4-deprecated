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

#include "scan.hpp"

using namespace std;
namespace fs = boost::filesystem;

Scan::Scan(std::string file_name, scan_opts opts) : file_name(file_name), opts(opts) {
  infile.open(file_name, std::fstream::binary);
  file_size = fs::file_size(file_name);
  buffer_size = opts.buffer_size;

  total_size = 0;
  if (file_size < buffer_size) {
    buffer_size = file_size;
  }
}

Scan::~Scan() {
  close();
}

void Scan::close() {
  if (infile.is_open()) {
    infile.close();
  }
}

bool Scan::run() {
  if (!infile.is_open()) {
    return false;
  }

  uintmax_t read_bytes = 0;
  char *buffer = new char[buffer_size];

  while (read_bytes < file_size) {
    if ((read_bytes + buffer_size) > file_size) {
      buffer_size = file_size - read_bytes;
      delete[] buffer;
      buffer = new char[buffer_size];
    }

    infile.read(buffer, buffer_size);

    // run scanners
    if (opts.enable_wav)
      riff_match(buffer, read_bytes);

    read_bytes += buffer_size;
  }

  delete[] buffer;
  return true;
}

bool Scan::is_riff_header(const char *header) {
  return memcmp(header, "RIFF", 4) == 0 && memcmp(header + 8, "WAVE", 4) == 0;
}

void Scan::riff_match(const char *buffer, uintmax_t current_offset) {
  wav_header *header;
  const uint bufsize = sizeof(wav_header);
  char *buf = new char[bufsize];
  bool change_pos = false;

  int index = charmatch(buffer, buffer_size, 'R');

  while (index != -1 && (uint)index <= buffer_size) {
    if (index + bufsize <= buffer_size) {
      std::memcpy(buf, buffer + index, bufsize);
    } else {
      infile.seekg(current_offset + index, std::ios::beg);
      infile.read(buf, bufsize);
      change_pos = true;
    }

    if (is_riff_header(buf)) {
      header = (wav_header*)(buf);

      stream_info si;
      si.file_type = types[riff];
      si.ext = exts[riff];
      si.file_size = header->wav_size + 8;
      si.offset = current_offset + index;
      si.data = header;
      total_size += si.file_size;
      stream_list.push_front(si);

      cout
        << boost::format("--> Found RIFF WAVE @ 0x%.8X (%s)")
          % (current_offset + index)
          % humnsize(si.file_size)
        << endl;
    }

    index = charmatch(buffer, buffer_size, 'R', index + 1);
  }

  delete[] buf;
  if (change_pos)
    infile.seekg(current_offset + buffer_size, std::ios::beg);
}

uintmax_t Scan::c_found_files() {
  return stream_list.size();
}

uintmax_t Scan::get_total_size() {
  return total_size;
}

std::list<stream_info> Scan::get_stream_list() {
  return stream_list;
}