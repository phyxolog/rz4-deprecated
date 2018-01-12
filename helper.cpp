#include "helper.hpp"

std::string Helper::humn_size(unsigned long long num) {
  const char *units[9] = { "B", "Kb", "Mb", "Gb", "Tb", "Pb", "Eb", "Zb", "Yb" };

  for (auto unit : units) {
    if (abs(num) < 1024.0) {
      return std::to_string(num) + " " + unit;
    }
    num /= 1024.0;
  }
}