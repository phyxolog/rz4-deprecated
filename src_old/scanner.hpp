// Copyright (c) 2018 Yura Zhivaga. All rights reserved.

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

#include "helper.hpp"
#include "types.hpp"

typedef struct ScanParams {
  bool enable_wav;
} ScanParams;

// struct of found signatures
// in `data` field save header or null
typedef struct Sign {
  std::string file_type;
  std::string ext;
  std::string short_type;
  size_t file_size;
  size_t offset;
  void *data;
} Sign;

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

class Scanner {
private:
  std::ifstream file;
  std::string file_name;
  uint buffer_size;
  uintmax_t file_size;
  uintmax_t current_offset;
  uintmax_t total_size;
  std::list<Sign> offset_list;
  ScanParams params;

  int search_char_in_buffer(const char*, uint, char, uint = 0);
public:
  Scanner(std::string, ScanParams, uint = 65536);
  ~Scanner();
  bool scan();
  std::list<Sign> get_offset_list();
  void close();
  uintmax_t get_count_of_found_files();
  uintmax_t get_total_size();

  // scanners
  void riff_wave_scanner(const char*, uintmax_t);

  // scanner helpers
  bool is_riff_wave_header(const char*);
};

#endif