#include "scanner.hpp"

using namespace std;

Scanner::Scanner(std::string file_name, unsigned int buffer_size = 65536) : file_name(file_name), buffer_size(buffer_size) {
  file.open(file_name, std::fstream::binary);
  file.seekg(0, std::ios::end);
  file_size = file.tellg();
  file.seekg(0, std::ios::beg);

  if (file_size < buffer_size) {
    buffer_size = file_size;
  }
}

int linear_search(const char *buffer, unsigned int buffer_size, char needle, unsigned int start_index = 0) {
  for (unsigned int i = start_index; i < buffer_size; i++) {
    if (buffer[i] == needle) {
      return i;
    }
  }

  return -1;
}

void Scanner::riff_wave_scanner(const char *buffer, unsigned long long current_offset) {
  int index = linear_search(buffer, buffer_size, 'R');
  unsigned long long offset = -1;
  char *buf = new char[12];

  while (index != -1) {
    offset = current_offset + index;

    file.seekg(offset, std::ios::beg);
    file.read(buf, 12);
    if (buf[0] == 0x52 && buf[1] == 0x49 && buf[2] == 0x46 && buf[3] == 0x46) {
      cout << "Some found!" << endl;
    }

    index = linear_search(buffer, buffer_size, 'R', index + 1);
  }

  delete[] buf;
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
    cout << read_bytes << " " << file_size << endl;
  }

  delete[] buffer;
}