// Copyright (c) 2018 Yura Zhivaga. All rights reserved.

#include <iostream>
#include <string>
#include <chrono>

#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"

#include "scanner.hpp"
#include "extractor.hpp"
#include "helper.hpp"
#include "args.hpp"

using namespace std;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

#define EXT_BUFFER_SIZE 262144
#define SCAN_BUFFER_SIZE 131072

#define OPTION_SCAN "s"
#define OPTION_COMPRESS "c"
#define OPTION_EXTRACT "e"

static const std::string logo =
"----------------------------------------------------------\n"
"|                           rz4                          |\n"
"|                     multimedia packer                  |\n"
"|            https://github.com/phyxolog/rz4_cpp         |\n"
"----------------------------------------------------------\n";

static const std::string usage_message =
"Usage:\n"
"    rz4 <command> [options] -i input_file [-o output_file] [-ext-dir extract_dir]\n\n"
"    Commands:\n"
"      c - compress input file.\n"
"      s - only scan input file.\n"
"      e - extract found signatures from input file.\n\n"
"    Detect options:\n"
"      --wav N - enable RIFF WAVE detect (default: 1).\n\n";

// Helper fuctions
bool check_arg_file(Args::Option* opt) {
  if (opt) {
    std::string file_path = (*opt).second;

    if (!fs::exists(file_path)) {
      return false;
    }
  } else {
    return false;
  }

  return true;
}

std::string timestamp_name(std::string prefix1, std::string prefix2) {
  return std::string(prefix1 + prefix2 + std::to_string(std::chrono::seconds(std::time(NULL)).count()));
}

std::chrono::high_resolution_clock::time_point now() {
  return std::chrono::high_resolution_clock::now();
}

int64_t duration(std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end) {
  return std::chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout << logo << usage_message << endl;
    return 0;
  }

  const fs::path workdir = fs::current_path();
  Scanner *scanner;
  Args ag(argc, argv);

  std::string command = argv[1], input_file;
  Args::Option* input_file_opt = ag.getParamFromKey("-i");

  cout << logo << endl;

  if (!check_arg_file(input_file_opt)) {
    cout << "[X] No such input file!" << endl;
    return 1;
  } else {
    input_file = (*input_file_opt).second;
  }

  ScanParams params;
  params.enable_wav = true;

  // get options
  Args::Option* enable_wav_opt = ag.getParamFromKey("--wav");

  if (enable_wav_opt) {
    params.enable_wav = (bool)stoi((*enable_wav_opt).second);
  }

  if (command.compare(OPTION_SCAN) == 0
      || command.compare(OPTION_COMPRESS) == 0
      || command.compare(OPTION_EXTRACT) == 0) {
    cout << "[!] Run scanner..." << endl << endl;

    scanner = new Scanner(input_file, params, SCAN_BUFFER_SIZE);
    auto start = now();
    scanner->scan();
    scanner->close();

    cout
      << boost::format("--> Found %d signatures, total size: %d")
        % scanner->get_count_of_found_files()
        % Helper::humn_size(scanner->get_total_size())
      << endl;

    cout << endl << "[!] Scanning duration: " << Helper::pretty_time(duration(start, now())) << endl;
  }
  
  if (command.compare(OPTION_EXTRACT) == 0) {
    Args::Option* extract_dir_opt = ag.getParamFromKey("-ext-dir");
    fs::path extract_dir = workdir / timestamp_name(input_file, "_extract_data_");

    if (extract_dir_opt) {
      extract_dir = (*extract_dir_opt).second;
    }

    if (!fs::exists(extract_dir)) {
      fs::create_directory(extract_dir);
    }

    cout << endl << "[!] Run extractor..." << endl;
    Extractor *extractor = new Extractor(input_file, EXT_BUFFER_SIZE);

    std::list<Sign> offset_list(scanner->get_offset_list());

    auto start = now();
    for (std::list<Sign>::const_iterator iterator = offset_list.begin(), end = offset_list.end(); iterator != end; ++iterator) {
      const fs::path path = extract_dir / boost::str(boost::format("%.8X-%.8X.%s")
                                            % (*iterator).offset
                                            % (*iterator).file_size
                                            % (*iterator).ext);

      extractor->extract((*iterator).offset, (*iterator).file_size, path.string());
    }

    extractor->close();
    cout << "[!] Extracting duration: " << Helper::pretty_time(duration(start, now())) << endl;
  }

  return 0;
}