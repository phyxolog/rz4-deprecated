#include <iostream>
#include <string>
#include <chrono>

#include <getopt.h>

#include "boost/filesystem.hpp"
#include "boost/format.hpp"

#include "scanner.hpp"
#include "extractor.hpp"
#include "helper.hpp"

#define BUFFER_SIZE 262144

#define COMMAND_SCAN "s"
#define COMMAND_COMPRESS "c"
#define COMMAND_EXTRACT "e"

#define see_help "See --help for usage information.\n"

typedef struct Options {
  boost::filesystem::path outdir;
  std::string command;
  std::string infile;
  std::string outfile;
  uint buffer_size;
} Options;

const struct option long_options[] = {
  {"compress", no_argument,       0, 'c' },
  {"extract",  no_argument,       0, 'e' },
  {"scan",     no_argument,       0, 's' },
  {"help",     no_argument,       0, 'h' },
  {"wav",      required_argument, 0, 'w' },
  {"input",    required_argument, 0, 'i' },
  {"output",   required_argument, 0, 'o' },
  {"outdir",   required_argument, 0, 'd' },
  {"bufsize",  required_argument, 0, 'b' },
	{0,          0,                 0,  0  }
};

static const std::string logo =
"----------------------------------------------------------\n"
"|                           rz4                          |\n"
"|                     multimedia packer                  |\n"
"|              https://github.com/phyxolog/rz4           |\n"
"----------------------------------------------------------\n";

static const std::string usage_message =
"Usage:\n"
"    rz4 <command> [options] --input=input_file [--output=output_file] [--outdir=output_dir]\n\n"
"    Commands:\n"
"      c - compress input file.\n"
"      s - only scan input file.\n"
"      e - extract found signatures from input file.\n\n"
"    Detect options:\n"
"      --wav=N     - enable RIFF WAVE detect (default: 1).\n\n"
"    Other options:\n"
"      --bufsize=N - set buffer size (default: 256kb).\n\n";