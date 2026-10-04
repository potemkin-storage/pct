// kiwinatra, 2026 (c)

#pragma once

#include <string>
#include <vector>
#include <map>

namespace parser {

    // what argv gave us, no guessing later
    struct ParsedCommand {
        std::string name;        // pull / update / remove
        std::string packet;      // packet name, empty for "update all"
        std::string output_dir;  // from -o, empty = root
        bool help = false;
        bool version = false;
        std::vector<std::string> rest; // unknown junk, so we can yell at the user
    };

    ParsedCommand parse_args(int argc, char** argv);

    // ~/.pctc and C:\.pctc shape
    struct Config {
        std::map<std::string, std::string> packets; // name -> version
        std::map<std::string, std::string> config;  // [config] key -> value
    };

    Config parse_pctc(const std::string& content);
    std::string serialize_pctc(const Config& cfg);

    // where the global file actually sits
    std::string pctc_path();

}