// kiwinatra, 2026 (c)

#pragma once

#include <string>
#include <vector>
#include <map>

namespace utils {

    // string stuff
    std::string trim(const std::string& s);
    std::vector<std::string> split(const std::string& s, char delim);
    bool starts_with(const std::string& s, const std::string& prefix);
    bool ends_with(const std::string& s, const std::string& suffix);

    // key=value lines, used for .pcmetadata and .pctc
    std::map<std::string, std::string> parse_kv(const std::string& content);

    // running stuff
    int run(const std::string& cmd);
    bool run_ok(const std::string& cmd);

    // asking the user
    std::string ask(const std::string& prompt);
    bool confirm(const std::string& prompt);

    // small printer, keeps output consistent
    void info(const std::string& msg);
    void ok(const std::string& msg);
    void warn(const std::string& msg);
    void err(const std::string& msg);

}