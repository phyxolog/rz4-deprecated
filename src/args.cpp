#include "args.hpp"

using namespace std;

Args::Args(int argc, char* argv[]) : argc_(argc), argv_(argv) {
  appName_ = argv_[0];
  this->parse();
}

Args::~Args() {
}

std::string Args::getAppName() const {
  return appName_;
}

void Args::parse() {
  typedef pair<string, string> Option;
  Option* option = new pair<string, string>();
  for (const char* const * i = this->begin() + 1; i != this->end(); i++) {
    const string p = *i;
    if (option->first == "" && p[0] == '-') {
      option->first = p;
      if (i == this->last()) {
        options_.insert(Option(option->first, option->second));
      }
      continue;
    } else if (option->first != "" && p[0] == '-') {
      option->second = "null"; /* or leave empty? */
      options_.insert(Option(option->first, option->second));
      option->first = p;
      option->second = "";
      if (i == this->last()) {
        options_.insert(Option(option->first, option->second));
      }
      continue;
    } else if (option->first != "") {
      option->second = p;
      options_.insert(Option(option->first, option->second));
      option->first = "";
      option->second = "";
      continue;
    }
  }
}

const char* const *Args::begin() const {
  return argv_;
}

const char* const *Args::end() const {
  return argv_ + argc_;
}

const char* const *Args::last() const {
  return argv_ + argc_ - 1;
}

bool Args::hasKey(const std::string& key) const {
  return options_.find(key) != options_.end();
}

Args::Option* Args::getParamFromKey(
  const std::string& key) const {
  const Options::const_iterator i = options_.find(key);
  Args::Option* o = 0;
  if (i != options_.end()) {
    o = new Args::Option((*i).first, (*i).second);
  }
  return o;
}