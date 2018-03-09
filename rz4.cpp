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
"      --wav=N - enable RIFF WAVE detect (default: 1).\n\n";

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

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout
      << logo
      << usage_message
      << endl;
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
  
  if (command.compare(OPTION_SCAN) == 0
      || command.compare(OPTION_COMPRESS) == 0
      || command.compare(OPTION_EXTRACT) == 0) {
    cout << "[!] Run scanner..." << endl << endl;

    scanner = new Scanner(input_file, SCAN_BUFFER_SIZE);
    auto start = std::chrono::high_resolution_clock::now();
    scanner->scan();
    auto end = std::chrono::high_resolution_clock::now();
    scanner->close();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout
      << boost::format("--> Found %d signatures, total size: %d")
        % scanner->get_count_of_found_files()
        % Helper::humn_size(scanner->get_total_size())
      << endl;

    cout << endl << "[!] Scanning duration: " << duration.count() << "ms" << endl;
  }
  
  if (command.compare(OPTION_EXTRACT) == 0) {
    Args::Option* extract_dir_opt = ag.getParamFromKey("-ext-dir");
    std::string current_timestamp_path_name(input_file + "_extract_data_" + std::to_string(std::chrono::seconds(std::time(NULL)).count()));
    fs::path extract_dir = workdir / current_timestamp_path_name;

    if (extract_dir_opt) {
      extract_dir = (*extract_dir_opt).second;
    }

    if (!fs::exists(extract_dir)) {
      fs::create_directory(extract_dir);
    }

    cout << endl << "[!] Run extractor..." << endl;
    Extractor *extractor = new Extractor(input_file, EXT_BUFFER_SIZE);

    std::list<Sign> offset_list(scanner->get_offset_list());

    auto start = std::chrono::high_resolution_clock::now();
    for (std::list<Sign>::const_iterator iterator = offset_list.begin(), end = offset_list.end(); iterator != end; ++iterator) {
      const fs::path path = extract_dir / boost::str(boost::format("%.8X-%.8X.%s")
                                            % (*iterator).offset
                                            % (*iterator).file_size
                                            % (*iterator).ext);

      extractor->extract((*iterator).offset, (*iterator).file_size, path.string());
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "[!] Extracting duration: " << duration.count() << "ms" << endl;
  }

  return 0;
}