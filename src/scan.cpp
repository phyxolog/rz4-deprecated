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
  if (header[0] == 0x52 && header[1] == 0x49 && header[2] == 0x46 && header[3] == 0x46
      && header[8] == 0x57 && header[9] == 0x41 && header[10] == 0x56 && header[11] == 0x45) {
    return true;
  }

  return false;
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