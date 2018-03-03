#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <cstring>
#include <list>

#include "boost/format.hpp"
#include "helper.hpp"
#include "types.hpp"

typedef struct _Sign {
  std::string file_type;
  std::string ext;
  std::string short_type;
  size_t file_size;
  size_t offset;
  void *data;
} Sign;

typedef struct wav_header {
  char riff_header[4];
  int wav_size;
  char wave_header[4];
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
  unsigned int buffer_size;
  unsigned long long file_size;
  unsigned long long current_offset;
  unsigned long long total_size;
  std::list<Sign> offset_list;

  int search_char_in_buffer(const char*, unsigned int, char, unsigned int = 0);
public:
  Scanner(std::string, unsigned int = 65536);
  ~Scanner();
  bool scan();
  unsigned long long get_count_of_found_files();
  unsigned long long get_total_size();

  // scanners
  void riff_wave_scanner(const char*, unsigned long long);
  // scanner helpers
  bool is_riff_wave_header(const char*);
};

#endif