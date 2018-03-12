#include "helper.hpp"

namespace fs = boost::filesystem;

std::string Helper::humn_size(uintmax_t bytes) {
  if (bytes == 0) return "0 B";
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

// nice time output, input t in ms
// 2^32 ms maximum, so will display incorrect negative values after about 49 days
// copy-pasted from https://github.com/schnaader/precomp-cpp/blob/master/precomp.cpp#L9613
std::string Helper::pretty_time(long long t) {
  if (t < 1000) {
    return boost::str(boost::format("%lims") % (long)t);
  } else if (t < 1000*60) {
    return boost::str(boost::format("%li second(s), %lims") % (long)(t / 1000) % (long)(t % 1000));
  } else if (t < 1000*60*60) {
    return boost::str(boost::format("%li minute(s), %li second(s)") % (long)(t / (1000 * 60)) % (long)((t / 1000) % 60));
  } else if (t < 1000*60*60*24) {
    return boost::str(boost::format("%li hour(s), %li minute(s), %li second(s)") % (long)(t / (1000*60*60)) % (long)((t / (1000*60)) % 60) % (long)((t / 1000) % 60));
  } else {
    return boost::str(boost::format("%li day(s), %li hour(s), %li minute(s)") % (long)(t / (1000*60*60*24)) % (long)((t / (1000*60*60)) % 24) % (long)((t / (1000*60)) % 60));    
  }
}