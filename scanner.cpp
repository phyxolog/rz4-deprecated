#include "scanner.hpp"

using namespace std;

Scanner::Scanner(std::string file_name, unsigned int buffer_size) : file_name(file_name), buffer_size(buffer_size) {
  file.open(file_name, std::fstream::binary);
  file.seekg(0, std::ios::end);
  file_size = file.tellg();
  file.seekg(0, std::ios::beg);
  total_size = 0;

  if (file_size < buffer_size) {
    buffer_size = file_size;
  }
}

int Scanner::search_char_in_buffer(const char *buffer, unsigned int buffer_size, char needle, unsigned int start_index) {
  const char* result = (const char*)std::memchr(buffer + start_index, needle, buffer_size);
  return result ? size_t(result - buffer) : -1;
}

bool Scanner::is_riff_wave_header(const char *header) {
  if (header[0] == 0x52 && header[1] == 0x49 && header[2] == 0x46 && header[3] == 0x46
      && header[8] == 0x57 && header[9] == 0x41 && header[10] == 0x56 && header[11] == 0x45) {
    return true;
  }

  return false;
}

void Scanner::riff_wave_scanner(const char *buffer, unsigned long long current_offset) {
  wav_header *header;
  const unsigned int bufsize = sizeof(wav_header);
  unsigned long long offset = -1;
  char *buf = new char[bufsize];
  bool change_pos = false;

  int index = search_char_in_buffer(buffer, buffer_size, 'R');

  while (index != -1 && (uint)index <= buffer_size) {
    if (index + bufsize <= buffer_size) {
      std::memcpy(buf, buffer + index, bufsize);
    } else {
      offset = current_offset + index;
      file.seekg(offset, std::ios::beg);
      file.read(buf, bufsize);
      change_pos = true;
    }

    if (is_riff_wave_header(buf)) {
      header = (wav_header*)(buf);
      Sign sign;
      sign.file_type = types[riff];
      sign.ext = exts[riff];
      sign.file_size = header->wav_size + 8;
      sign.offset = current_offset + index;
      sign.data = header;
      total_size += sign.file_size;
      offset_list.push_back(sign);

      cout
        << boost::format("--> Found RIFF WAVE @ 0x%.8X (%s)")
          % (current_offset + index)
          % Helper::humn_size(sign.file_size)
        << endl;
    }

    index = search_char_in_buffer(buffer, buffer_size, 'R', index + 1);
  }

  delete[] buf;
  if (change_pos)
    file.seekg(current_offset + buffer_size, std::ios::beg);
}

bool Scanner::scan() {
  if (!file.is_open()) {
    return false;
  }

  unsigned long long read_bytes = 0, current_offset = 0;
  char *buffer = new char[buffer_size];

  while (read_bytes < file_size) {
    if ((read_bytes + buffer_size) > file_size) {
      buffer_size = file_size - read_bytes;
      delete[] buffer;
      buffer = new char[buffer_size];
    }

    file.read(buffer, buffer_size);
    current_offset = read_bytes;

    // run scanners
    riff_wave_scanner(buffer, current_offset);

    read_bytes += buffer_size;
  }

  delete[] buffer;
  return true;
}

unsigned long long Scanner::get_count_of_found_files() {
  return offset_list.size();
}

unsigned long long Scanner::get_total_size() {
  return total_size;
}

Scanner::~Scanner() {
  file.close();
}