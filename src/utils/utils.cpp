// kiwinatra, 2026 (c)

#include "utils.h"
#include "colors.h"

#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdlib>

namespace utils {

    std::string trim(const std::string& s) {
        size_t a = 0;
        size_t b = s.size();
        while (a < b && std::isspace((unsigned char)s[a])) a++;
        while (b > a && std::isspace((unsigned char)s[b - 1])) b--;
        return s.substr(a, b - a);
    }

    std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> out;
        std::string cur;
        for (char c : s) {
            if (c == delim) {
                out.push_back(cur);
                cur.clear();
            } else {
                cur.push_back(c);
            }
        }
        out.push_back(cur);
        return out;
    }

    bool starts_with(const std::string& s, const std::string& prefix) {
        return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
    }

    bool ends_with(const std::string& s, const std::string& suffix) {
        return s.size() >= suffix.size() &&
               s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    // flat key=value, blanks and #comments dropped
    std::map<std::string, std::string> parse_kv(const std::string& content) {
        std::map<std::string, std::string> out;
        std::string line;
        auto flush = [&]() {
            std::string t = trim(line);
            line.clear();
            if (t.empty() || t[0] == '#') return;
            auto eq = t.find('=');
            if (eq == std::string::npos) return;
            out[trim(t.substr(0, eq))] = trim(t.substr(eq + 1));
        };
        for (char c : content) {
            if (c == '\n') flush();
            else line.push_back(c);
        }
        flush();
        return out;
    }

    int run(const std::string& cmd) {
        return std::system(cmd.c_str());
    }

    bool run_ok(const std::string& cmd) {
        return run(cmd) == 0;
    }

    std::string ask(const std::string& prompt) {
        std::cout << prompt;
        std::cout.flush();
        std::string line;
        std::getline(std::cin, line);
        return trim(line);
    }

    // yes / y = true, anything else = false
    bool confirm(const std::string& prompt) {
        std::string a = ask(prompt + " [y/N] ");
        if (a.empty()) return false;
        return a[0] == 'y' || a[0] == 'Y';
    }

    void info(const std::string& msg) {
        std::cout << colors::cyan(":: ") << msg << "\n";
    }

    void ok(const std::string& msg) {
        std::cout << colors::green("ok ") << msg << "\n";
    }

    void warn(const std::string& msg) {
        std::cout << colors::yellow("!! ") << msg << "\n";
    }

    void err(const std::string& msg) {
        std::cerr << colors::red("xx ") << msg << "\n";
    }

}