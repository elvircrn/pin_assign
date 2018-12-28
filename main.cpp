/**
 *  pin_assign
 * Result:
 *  Parses non-recursively all *.vhd files for port assignment tags
 *
 *  pin_assgn test.vhd
 * Result:
 *  Finds all port assignment tags in test.vhd located in the current directory.
 *
 *  pin_assign ./path_to_folder
 * Result:
 *  Finds all port assignment tags non-recursively in all files ending .vhd witin './path_to_folder'.
 */
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cctype>

namespace fs = std::filesystem;

void parsePort(std::ifstream &vhdl, std::string &lineBuff);

void parseEntity(std::ifstream &vhdl, const std::string &lineBuff) {
  std::istringstream iss(lineBuff);

  std::string buffer;
  while (iss >> buffer) {
    if (buffer != "entity") {
      std::cout << "Parsing entity " << buffer << '\n';
      return;
    }
  }
}
/**
 * Example:
 *   "   aaaa " -> "aaaa"
 *   " a a " -> "a a"
 */
std::string trimLazy(const std::string &s) {
  std::string ret;
  auto it = s.begin();
  for (; it != s.end() && std::isspace(*it); it++) {
  }

  for (; it != s.end() && !std::isspace(*it); it++) {
    ret += *it;
  }

  return ret;
}
void parseFile(std::ifstream &vhdl) {
  std::string lineBuff;
  while (std::getline(vhdl, lineBuff)) {
    std::istringstream iss(lineBuff);

    std::string buffer;
    iss >> buffer;

    if (buffer.find("port") != std::string::npos) {
      parsePort(vhdl, lineBuff);
    } else if (buffer.find("entity") != std::string::npos) {
      parseEntity(vhdl, lineBuff);
    }
  }
}

void parsePort(std::ifstream &vhdl, std::string &lineBuff) {
  while (getline(vhdl, lineBuff) && trimLazy(lineBuff) != ");") {
    std::istringstream portLine(lineBuff);
    std::string buff;
    portLine >> buff;
    std::string portId = buff.substr(0, buff.size() - 1);

    bool isVector = false;
    while (portLine >> buff) {
      isVector |= (buff.find("std_logic_vector") != std::string::npos);
      if (buff == "@") {
        std::vector<std::string> ports;
        while (portLine >> buff) {
          ports.push_back(buff);
        }

        int id{};
        for (const auto &port : ports) {
          std::cout << " 	set_location_assignment PIN_" << port << " -to " << portId
                    << (isVector ? "[" + port + "]" : "") << '\n';
          id++;
        }
        std::cout << "\n";
      }
    }
  }
}

int main(int argc, char **argv) {

  // No files passed, parse all files in current directory
  if (argc == 1) {
    for (const auto &entry : fs::directory_iterator(fs::current_path())) {
      if (entry.path().extension() == ".vhd") {
        auto vhdl = std::ifstream(entry.path());
        parseFile(vhdl);
      }
    }
  } else if (std::filesystem::is_directory(argv[1])) { // Scan in current directory
    for (const auto &entry : fs::directory_iterator(argv[1])) {
      if (entry.path().extension() == ".vhd") {
        auto vhdl = std::ifstream(entry.path());
        parseFile(vhdl);
      }
    }
  } else { // Scan in file
    auto vhdl = std::ifstream(argv[1]);
    parseFile(vhdl);
  }

  return 0;
}
