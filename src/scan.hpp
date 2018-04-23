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

/*
* Struct of RIFF WAVE format
*/
typedef struct wav_header {
  char  riff_header[4]; /* RIFF */
  int   wav_size;       /* File size = `wav_size` + 8 */
  char  wave_header[4]; /* WAVE */
  char  fmt_header[4];
  int   fmt_chunk_size;
  short audio_format;
  short num_channels;
  int   sample_rate;
  int   byte_rate;
  short sample_alignment;
  short bit_depth; 
  char  data_header[4];
  int   data_bytes;
} wav_header;

/*
* Struct of BITMAM format
*/
typedef struct bmp_header {
  uint16_t magic;
  uint32_t filesize;
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t dataoffset;
  uint32_t headersize;
  int32_t  width;
  int32_t  height;
  uint16_t planes;
  uint16_t bpp;
  uint32_t compression;
  uint32_t datasize;
  uint32_t hres;
  uint32_t vres;
  uint32_t palettecolors;
  uint32_t importantcolors;
} bmp_header;

class Scan {
private:
  std::ifstream infile;
  std::string file_name;
  unsigned int buffer_size;
  uintmax_t file_size;
  uintmax_t current_offset;
  uintmax_t total_size;
  std::list<StreamInfo> stream_list;
  Options opts;

public:
  Scan(Options);
  ~Scan();
  bool run();
  void close();
  std::list<StreamInfo> get_stream_list();
  uintmax_t c_found_files();
  uintmax_t get_total_size();

  // scanners
  bool is_riff_header(const char*);
  void riff_match(const char*, uintmax_t);  
};

#endif