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

  // init options
  Options options;
  options.command = command;
  options.buffer_size = BUFFER_SIZE;

  // init default params for scanner
  scan_opts scan_opts;
  scan_opts.enable_wav = 1;

  Scan *scanner;

  int opt = 0;

  while ((opt = getopt_long(argc, argv, "w:h:i:o:d:b", long_options, NULL)) != -1) {
    switch (opt) {
      case 'w':
        scan_opts.enable_wav = parse_bool(optarg);
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

  scan_opts.buffer_size = options.buffer_size;

  cout << "-> Buffer size: " << humnsize(options.buffer_size) << endl;

  scanner = new Scan(options.infile, scan_opts);
  
  if (options.command.compare(COMMAND_SCAN) == 0
      || options.command.compare(COMMAND_COMPRESS) == 0
      || options.command.compare(COMMAND_EXTRACT) == 0) {
    cout << "-> Run scanner..." << endl << endl;

    scanner->run();
    scanner->close();

    cout
      << boost::format("--> Found %d signatures, total size: %d")
        % scanner->c_found_files()
        % humnsize(scanner->get_total_size())
      << endl;
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

// generate name with prefixes
static std::string gennamep(std::string prefix1, std::string prefix2) {
  return string(prefix1 + prefix2 + to_string(chrono::seconds(std::time(NULL)).count()));
}