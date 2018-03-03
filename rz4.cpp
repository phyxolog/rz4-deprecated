#include <iostream>

#include "boost/format.hpp"

#include "scanner.hpp"
#include "helper.hpp"

using namespace std;

#define SCAN_BUFFER_SIZE 131072

int main(int argc, char *argv[]) {
  cout
    << "rz4 - multimedia packer" << endl
    << "author: phyxolog" << endl
    << "https://github.com/phyxolog/rz4_cpp"
    << endl << endl;

  cout << "[!] Run scanner..." << endl;
  Scanner *scanner = new Scanner("media.data", SCAN_BUFFER_SIZE);
  scanner->scan();

  cout
    << boost::format("--> Found %d signatures, total size: %d")
      % scanner->get_count_of_found_files()
      % Helper::humn_size(scanner->get_total_size())
    << endl;

  return 0;
}