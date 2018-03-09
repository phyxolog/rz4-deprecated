// From https://stackoverflow.com/questions/253556/what-parameter-parser-libraries-are-there-for-c

#ifndef ARGS_H
#define ARGS_H

#include <map>
#include <string>
#include <iostream>

class Args {
public:
  typedef std::pair<std::string, std::string> Option;
  Args(int argc, char *argv[]);
  virtual ~Args();
  std::string getAppName() const;
  bool hasKey(const std::string&) const;
  Option* getParamFromKey(const std::string&) const;
private:
  typedef std::map<std::string, std::string> Options;
  void parse();
  const char* const *begin() const;
  const char* const *end() const;
  const char* const *last() const;
  Options options_;
  int argc_;
  char** argv_;
  std::string appName_;
};

#endif