#include "util.hpp"

using namespace std; // remove this after tests
namespace fs = boost::filesystem;

std::string humnsize(uintmax_t bytes) {
  if (bytes == 0) return "0 B";
  int exp = (int)(log(bytes) / log(1024));
  std::string prefix = std::string("BKMGTPE").substr(exp, 1) + (exp == 0 ? "" : "b");
  return boost::str(boost::format("%.2f %s") % (bytes / pow(1024, exp)) % prefix);
}

std::string tmpfile(const std::string& path) {
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
std::string prettytime(long long time) {
  long t = (long)time;

  if (t < 1000) {
    return boost::str(boost::format("%lims") % t);
  } else if (t < 1000 * 60) {
    return boost::str(boost::format("%li second(s), %lims") % (t / 1000) % (t % 1000));
  } else if (t < 1000 * 60 * 60) {
    return boost::str(boost::format("%li minute(s), %li second(s)")
                      % (t / (1000 * 60))
                      % ((t / 1000) % 60));
  } else if (t < 1000 * 60 * 60 * 24) {
    return boost::str(boost::format("%li hour(s), %li minute(s), %li second(s)")
                      % (t / (1000 * 60 * 60))
                      % ((t / (1000 * 60)) % 60)
                      % ((t / 1000) % 60));
  } else {
    return boost::str(boost::format("%li day(s), %li hour(s), %li minute(s)")
                      % (t / (1000 * 60 * 60 * 24))
                      % ((t / (1000 *60 * 60)) % 24)
                      % ((t / (1000 * 60)) % 60));    
  }
}

/*
* Convert a string representing an amount of memory into the number of
* bytes, so for instance memtoll("1Gb") will return 1073741824 that is
* (1024*1024*1024).
*/
uintmax_t memtoll(std::string str) {
  long mul = 0;
  size_t nondigit_pos = str.find_first_not_of("0123456789");
  std::string digits = str.substr(0, nondigit_pos);
  std::string u = str.substr(nondigit_pos, str.length());
  std::transform(u.begin(), u.end(), u.begin(), ::tolower);

  std::map <std::string, long> umul = {
    { "b",  1                   },
    { "k",  1000                },
    { "kb", 1024                },
    { "m",  1000 * 1000         },
    { "mb", 1024 * 1024         },
    { "g",  1000L * 1000 * 1000 },
    { "gb", 1024L * 1024 * 1024 },
  };
  
  if (digits.length() == 0 || umul.find(u) == umul.end()) {
    return 0;
  } else {
    mul = umul.at(u);
    return std::stoll(digits) * mul;
  }
}

/*
* Find char in array
* if fouund - return index
* else return -1
*/
int charmatch(const char *buffer, uint buffer_size, char needle, uint offset) {
  if (offset >= buffer_size) {
    return -1;
  }

  buffer_size -= offset;
  const char* result = (const char*)std::memchr(buffer + offset, needle, buffer_size);
  return result ? size_t(result - buffer) : -1;
}