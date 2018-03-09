#include "scanner.hpp"

using namespace std;
namespace fs = boost::filesystem;

Scanner::Scanner(std::string file_name, uint buffer_size) : file_name(file_name), buffer_size(buffer_size) {
  file.open(file_name, std::fstream::binary);
  file_size = fs::file_size(file_name);
  total_size = 0;

  if (file_size < buffer_size) {
    buffer_size = file_size;
  }
}

// if found - return index
// else return -1
int Scanner::search_char_in_buffer(const char *buffer, uint buffer_size, char needle, uint start_index) {
  if (start_index > buffer_size) {
    return -1;
  }

  for (uint i = start_index; i < buffer_size; i++) {
    if (buffer[i] == needle) {
      return i;
    }
  }

  return -1;
}

bool Scanner::is_riff_wave_header(const char *header) {
  if (header[0] == 0x52 && header[1] == 0x49 && header[2] == 0x46 && header[3] == 0x46
      && header[8] == 0x57 && header[9] == 0x41 && header[10] == 0x56 && header[11] == 0x45) {
    return true;
  }

  return false;
}

void Scanner::riff_wave_scanner(const char *buffer, uintmax_t current_offset) {
  wav_header *header;
  const uint bufsize = sizeof(wav_header);
  char *buf = new char[bufsize];
  bool change_pos = false;

  int index = search_char_in_buffer(buffer, buffer_size, 'R');

  while (index != -1 && (uint)index <= buffer_size) {
    if (index + bufsize <= buffer_size) {
      std::memcpy(buf, buffer + index, bufsize);
    } else {
      file.seekg(current_offset + index, std::ios::beg);
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
      offset_list.push_front(sign);

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

  uintmax_t read_bytes = 0, current_offset = 0;
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

uintmax_t Scanner::get_count_of_found_files() {
  return offset_list.size();
}

uintmax_t Scanner::get_total_size() {
  return total_size;
}

std::list<Sign> Scanner::get_offset_list() {
  return offset_list;
}

void Scanner::close() {
  if (file.is_open()) {
    file.close();
  }
}

Scanner::~Scanner() {
  file.close();
}