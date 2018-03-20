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

// copy-pasted from https://github.com/antirez/redis/blob/398b2084af067ae4d669e0ce5a63d3bc89c639d3/src/util.c#L180
/* Convert a string representing an amount of memory into the number of
 * bytes, so for instance memtoll("1Gb") will return 1073741824 that is
 * (1024*1024*1024).
 *
 * On parsing error, if *err is not NULL, it's set to 1, otherwise it's
 * set to 0. On error the function return value is 0, regardless of the
 * fact 'err' is NULL or not. */
uintmax_t memtoll(const char *p) {
  const char *u;
  char buf[128];
  long mul; /* unit multiplier */
  long long val;
  unsigned int digits;

  /* Search the first non digit character. */
  u = p;
  if (*u == '-') u++;
  while(*u && isdigit(*u)) u++;

  if (*u == '\0' || !strcasecmp(u, "b")) {
    mul = 1;
  } else if (!strcasecmp(u, "k")) {
    mul = 1000;
  } else if (!strcasecmp(u, "kb")) {
    mul = 1024;
  } else if (!strcasecmp(u, "m")) {
    mul = 1000 * 1000;
  } else if (!strcasecmp(u, "mb")) {
    mul = 1024 * 1024;
  } else if (!strcasecmp(u, "g")) {
    mul = 1000L * 1000 * 1000;
  } else if (!strcasecmp(u, "gb")) {
    mul = 1024L * 1024 * 1024;
  } else {
    return 0;
  }

  /* Copy the digits into a buffer, we'll use strtoll() to convert
    * the digit (without the unit) into a number. */
  digits = u - p;

  if (digits >= sizeof(buf)) {
    return 0;
  }

  memcpy(buf, p, digits);
  buf[digits] = '\0';

  char *endptr;
  errno = 0;
  val = strtoll(buf, &endptr, 10);

  if ((val == 0 && errno == EINVAL) || *endptr != '\0') {
    return 0;
  }

  return val*mul;
}

// if found - return index
// else return -1
int charmatch(const char *buffer, uint buffer_size, char needle, uint offset) {
  if (offset >= buffer_size) {
    return -1;
  }

  buffer_size -= offset;
  const char* result = (const char*)std::memchr(buffer + offset, needle, buffer_size);
  return result ? size_t(result - buffer) : -1;
}