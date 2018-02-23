#include "helper.hpp"

using namespace std;

std::string Helper::humn_size(unsigned long long bytes) {
  int exp = (int) (log(bytes) / log(1024));
  bool zer0 = exp - 1 == 0;
  std::string prefix = std::string("BKMGTPE").substr(zer0 ? 0 : exp - 1, 1) + (zer0 ? "" : "b");
  return boost::str(boost::format("%.2f %s") % (bytes / pow(1024, exp)) % prefix);
}