/*
 * Copyright (C) 2018 Yura Zhivaga <yzhivaga@gmail.com>
 * 
 * This file is part of rz4.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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

/*
* Convert ms to human-oriented time string
*/
std::string prettytime(uintmax_t time) {
  const int size = 4;
  int step = -1;
  uintmax_t steps[size] = {
    1000,
    1000 * 60,
    1000 * 60 * 60,
    1000 * 60 * 60 * 24
  };

  for (int i = 0; i < size; i++) {
    if (time < steps[i]) {
      step = i;
      break;
    }
  }

  switch (step) {
    case 0:
      return boost::str(boost::format("%lims") % time);
      break;
    case 1: // only ms
      return boost::str(boost::format("%li second(s), %lims") % (time / 1000) % (time % 1000));
      break;
    case 2: // minutes
      return boost::str(boost::format("%li minute(s), %li second(s)")
                        % (time / (1000 * 60))
                        % ((time / 1000) % 60));
      break;
    case 3: // hours
      return boost::str(boost::format("%li hour(s), %li minute(s), %li second(s)")
                        % (time / (1000 * 60 * 60))
                        % ((time / (1000 * 60)) % 60)
                        % ((time / 1000) % 60));
      break;
    default: // max (days)
      return boost::str(boost::format("%li day(s), %li hour(s), %li minute(s)")
                        % (time / (1000 * 60 * 60 * 24))
                        % ((time / (1000 *60 * 60)) % 24)
                        % ((time / (1000 * 60)) % 60));
  }
}

/*
* Convert a string representing an amount of memory into the number of
* bytes, so for instance memtoll("1Gb") will return 1073741824 that is
* (1024*1024*1024).
*/
uintmax_t memtoll(std::string str) {
  if (str.length() == 0) {
    return 0;
  }

  size_t nondigit_pos = str.find_first_not_of("0123456789");

  if (nondigit_pos == std::string::npos) {
    return std::stoll(str);
  }

  uintmax_t result = 0;
  std::string digits = str.substr(0, nondigit_pos);
  std::string u = str.substr(nondigit_pos, str.length());
  std::transform(u.begin(), u.end(), u.begin(), ::tolower);

  if (digits.length() == 0) {
    return result;
  } else {
    result = std::stoll(digits);
  }

  std::map<std::string, long> umul = {
    { "b",  1                   },
    { "k",  1000                },
    { "kb", 1024                },
    { "m",  1000  * 1000        },
    { "mb", 1024  * 1024        },
    { "g",  1000L * 1000 * 1000 },
    { "gb", 1024L * 1024 * 1024 },
  };
  
  auto mul = umul.find(u);
  if (mul != umul.end()) {
    return result * mul->second;
  } else {
    return result;
  }
}

/*
* Find char in array
* if found - return index
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