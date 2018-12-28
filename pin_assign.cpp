#include <iostream>
#include <string>
#include <cstring>
#include <ifstream>
#include <sstream>
#include <cctype>

/**
 * Example:
 *   "   aaaa " -> "aaaa"
 *   " a a " -> "a a"
 */
std::string trimLazy(const std::string &s) {
  std::string ret;
  auto& it = s.begin();
  for (; it != s.end() && std::isspace(*it); it++) { }

  for (; it != s.end() && !std::isspace(*it); it++) {
    ret += *it;
  }

  return ret;
}

int main(int argc, char** argv) {
  auto vhdl = std::ifstream(argv[1]);

  std::string lineBuff;
  while (std::getline(vhdl, lineBuff)) {
    std::istringstream iss(lineBuff);

    std::string identifier;

    iss >> identifier;

    if (identifier.find("port") != std::string::npos) {
      while () {
      }
      break;
    }
  }

  return 0;
}
