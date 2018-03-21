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

#include "main.hpp"

using namespace std;
namespace fs = boost::filesystem;

static std::string gennamep(std::string, std::string);
static bool parse_bool(char*);
static int usage();

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return usage();
  }

  // get current path
  const fs::path workdir = fs::current_path();

  // `command` always the first argument
  std::string command = argv[1];
  std::transform(command.begin(), command.end(), command.begin(), ::tolower);

  // init options
  Options options;
  options.command = command;
  options.buffer_size = BUFFER_SIZE;

  // init default params for scanner
  scan_opts scan_opts;
  scan_opts.enable_wav = 1;

  // input file always the last arg
  options.infile = argv[argc - 1];

  Scan  *scanner;
  Eject *ejector;

  int opt = 0;

  while ((opt = getopt_long(argc, argv, "w:h:i:o:d:b", long_options, NULL)) != -1) {
    switch (opt) {
      case 'w':
        scan_opts.enable_wav = parse_bool(optarg);
        break;

      case 'h':
        return usage();

      case 'o':
        options.outfile = optarg;
        break;

      case 'd':
        options.outdir = optarg;
        break;

      case 'b':
        options.buffer_size = memtoll(optarg);
        break;

      default:
        std::printf("%s\n", see_help);
        return 1;
    }
  }

  cout << logo << endl;

  if (options.buffer_size <= 0) {
    options.buffer_size = BUFFER_SIZE;
  }

  if (options.infile.empty() || !fs::exists(options.infile)) {
    cout << "[!] No such input file!" << endl;
    return 1;
  }

  if (options.command == COMMAND_EXTRACT
      && options.outdir.empty()) {
    options.outdir = workdir / gennamep(options.infile, "_extract_data_");
  }

  if (options.command == COMMAND_EXTRACT
      && !fs::exists(options.outdir)) {
    fs::create_directory(options.outdir);
  }

  cout << "-> Buffer size: " << humnsize(options.buffer_size) << endl;

  scan_opts.buffer_size = options.buffer_size;
  scanner = new Scan(options.infile, scan_opts);
  
  if (options.command.compare(COMMAND_SCAN) == 0
      || options.command.compare(COMMAND_COMPRESS) == 0
      || options.command.compare(COMMAND_EXTRACT) == 0) {
    cout << "-> Scanning..." << endl << endl;

    scanner->run();
    scanner->close();

    cout
      << boost::format("--> Found %d signatures, total size: %d")
        % scanner->c_found_files()
        % humnsize(scanner->get_total_size())
      << endl;
  }

  if (options.command.compare(COMMAND_EXTRACT) == 0) {
    cout << endl << "-> Extract data..." << endl;

    ejector = new Eject(options.infile, options.buffer_size);
    std::list<stream_info> stream_list(scanner->get_stream_list());

    uintmax_t i = 1, count = scanner->c_found_files();
    std::list<stream_info>::const_iterator iter, end; 
    
    for (iter = stream_list.begin(), end = stream_list.end(); iter != end; iter++, i++) {
      const fs::path path = options.outdir / boost::str(boost::format("%016X-%016X.%s")
                                            % (*iter).offset
                                            % (*iter).file_size
                                            % (*iter).ext);

      ejector->extract((*iter).offset, (*iter).file_size, path.string());
      std::cout << "\r" << "-> " << i * 100 / count << "% completed.";
    }

    ejector->close();

    cout << endl << "-> Extracting successfully!" << endl;
  }

  return 0;
}

static bool parse_bool(char *str) {
  return (bool)atoi(str);
}

static int usage() {
  cout << logo << usage_message << endl;
  return 1;
}

/*
* Generate file name with prefixes
*/
static std::string gennamep(std::string prefix1, std::string prefix2) {
  return string(prefix1 + prefix2 + to_string(chrono::seconds(std::time(NULL)).count()));
}