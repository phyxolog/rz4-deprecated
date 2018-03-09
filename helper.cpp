#include "helper.hpp"

namespace fs = boost::filesystem;

std::string Helper::humn_size(uintmax_t bytes) {
  int exp = (int)(log(bytes) / log(1024));
  std::string prefix = std::string("BKMGTPE").substr(exp, 1) + (exp == 0 ? "" : "b");
  return boost::str(boost::format("%.2f %s") % (bytes / pow(1024, exp)) % prefix);
}

std::string Helper::generate_temp_file_name(const std::string& path) {
  std::string mask = "~temp%09d.dat";
  fs::path full_path, temp;
  int i = 1;

  while (true) {
    temp = boost::str(boost::format(mask) % i);
    full_path = path / temp;
    if (!fs::exists(full_path)) {
      break;
    }

    i++;
  }

  return full_path.string();
}