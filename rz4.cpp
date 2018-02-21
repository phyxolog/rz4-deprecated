#include <iostream>

#include "scanner.hpp"
#include "helper.hpp"

using namespace std;

#define SCAN_BUFFER_SIZE 65535

int main(int argc, char *argv[]) {
  Scanner *scanner = new Scanner("media.data", SCAN_BUFFER_SIZE);
  scanner->scan();
  // std::string test = Helper::humn_size(125);
  // cout << test << endl;
  return 0;
}