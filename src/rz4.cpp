// Copyright (c) 2018 Yura Zhivaga. All rights reserved.

#include "rz4.hpp"

using namespace std;
namespace fs = boost::filesystem;

static chrono::high_resolution_clock::time_point now();
static std::string timestamp_name(std::string, std::string);
static int64_t duration(chrono::high_resolution_clock::time_point, chrono::high_resolution_clock::time_point);
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

  // init options
  Options options;
  options.command = command;
  options.buffer_size = BUFFER_SIZE;

  // init default params for scanner
  ScanParams params;
  params.enable_wav = 1;

  Scanner *scanner;
  Extractor *extractor;

  int opt = 0;

  while ((opt = getopt_long(argc, argv, "w:h:i:o:d:b", long_options, NULL)) != -1) {
    switch (opt) {
      case 'w':
        params.enable_wav = parse_bool(optarg);
        break;

      case 'h':
        return usage();

      case 'i':
        options.infile = optarg;
        break;

      case 'o':
        options.outfile = optarg;
        break;

      case 'd':
        options.outdir = optarg;
        break;

      case 'b':
        options.buffer_size = Helper::memtoll(optarg);
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
    options.outdir = workdir / timestamp_name(options.infile, "_extract_data_");
  }

  if (options.command == COMMAND_EXTRACT
      && !fs::exists(options.outdir)) {
    fs::create_directory(options.outdir);
  }

  cout << "-> Buffer size: " << Helper::humn_size(options.buffer_size) << endl;

  scanner = new Scanner(options.infile, params, options.buffer_size);

  if (options.command.compare(COMMAND_SCAN) == 0
      || options.command.compare(COMMAND_COMPRESS) == 0
      || options.command.compare(COMMAND_EXTRACT) == 0) {
    cout << "-> Run scanner..." << endl << endl;

    auto start = now();
    scanner->scan();
    scanner->close();

    cout
      << boost::format("--> Found %d signatures, total size: %d")
        % scanner->get_count_of_found_files()
        % Helper::humn_size(scanner->get_total_size())
      << endl;

    cout << endl << "-> Scanning duration: " << Helper::pretty_time(duration(start, now())) << endl;
  }

  if (command.compare(COMMAND_EXTRACT) == 0) {
    cout << endl << "-> Run extractor..." << endl;
    extractor = new Extractor(options.infile, options.buffer_size);

    std::list<Sign> offset_list(scanner->get_offset_list());

    auto start = now();
    for (std::list<Sign>::const_iterator iterator = offset_list.begin(), end = offset_list.end(); iterator != end; ++iterator) {
      const fs::path path = options.outdir / boost::str(boost::format("%.8X-%.8X.%s")
                                            % (*iterator).offset
                                            % (*iterator).file_size
                                            % (*iterator).ext);

      extractor->extract((*iterator).offset, (*iterator).file_size, path.string());
    }

    extractor->close();
    cout << "-> Extracting duration: " << Helper::pretty_time(duration(start, now())) << endl;
  }
}

static std::string timestamp_name(std::string prefix1, std::string prefix2) {
  return string(prefix1 + prefix2 + to_string(chrono::seconds(std::time(NULL)).count()));
}

static chrono::high_resolution_clock::time_point now() {
  return chrono::high_resolution_clock::now();
}

static int64_t duration(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point end) {
  return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

static bool parse_bool(char *str) {
  return (bool)atoi(str);
}

static int usage() {
  cout << logo << usage_message << endl;
  return 1;
}