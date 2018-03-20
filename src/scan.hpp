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

#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <cstring>
#include <list>

#include "boost/filesystem.hpp"
#include "boost/format.hpp"

#include "util.hpp"
#include "types.hpp"

typedef struct scan_opts {
  uint buffer_size;
  bool enable_wav;
} scan_opts;

// struct of RIFF WAVE format
typedef struct wav_header {
  char riff_header[4]; // `RIFF`
  int wav_size; // file_size = `wav_size` + 8
  char wave_header[4]; // `WAVE`
  char fmt_header[4];
  int fmt_chunk_size;
  short audio_format;
  short num_channels;
  int sample_rate;
  int byte_rate;
  short sample_alignment;
  short bit_depth; 
  char data_header[4];
  int data_bytes;
} wav_header;

class Scan {
private:
  std::ifstream infile;
  std::string file_name;
  uint buffer_size;
  uintmax_t file_size;
  uintmax_t current_offset;
  uintmax_t total_size;
  std::list<stream_info> stream_list;
  scan_opts opts;

public:
  Scan(std::string, scan_opts);
  ~Scan();
  bool run();
  void close();
  std::list<stream_info> get_stream_list();
  uintmax_t c_found_files();
  uintmax_t get_total_size();

  // scanners
  bool is_riff_header(const char*);
  void riff_match(const char*, uintmax_t);  
};

#endif