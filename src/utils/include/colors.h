// kiwinatra, 2026 (c)

#pragma once

#include <string>
#include <cstdlib>
#include <cstdio>

#ifdef _WIN32
#include <io.h>
#define PCT_ISATTY _isatty
#define PCT_FILENO _fileno
#else
#include <unistd.h>
#define PCT_ISATTY isatty
#define PCT_FILENO fileno
#endif

namespace colors {

    inline const std::string RESET   = "\033[0m";
    inline const std::string RED     = "\033[31m";
    inline const std::string GREEN   = "\033[32m";
    inline const std::string YELLOW  = "\033[33m";
    inline const std::string BLUE    = "\033[34m";
    inline const std::string MAGENTA = "\033[35m";
    inline const std::string CYAN    = "\033[36m";
    inline const std::string BOLD    = "\033[1m";
    inline const std::string DIM     = "\033[2m";

    // working fr ig bro
    inline bool enabled() {
        static int cached = -1;
        if (cached != -1) return cached == 1;

        const char* no_color = std::getenv("NO_COLOR");
        if (no_color && *no_color) {
            cached = 0;
            return false;
        }

        cached = PCT_ISATTY(PCT_FILENO(stdout)) ? 1 : 0;
        return cached == 1;
    }

    inline std::string wrap(const std::string& code, const std::string& s) {
        if (!enabled()) return s;
        return code + s + RESET;
    }

    inline std::string red(const std::string& s)     { return wrap(RED, s); }
    inline std::string green(const std::string& s)   { return wrap(GREEN, s); }
    inline std::string yellow(const std::string& s)  { return wrap(YELLOW, s); }
    inline std::string blue(const std::string& s)    { return wrap(BLUE, s); }
    inline std::string magenta(const std::string& s) { return wrap(MAGENTA, s); }
    inline std::string cyan(const std::string& s)    { return wrap(CYAN, s); }
    inline std::string bold(const std::string& s)    { return wrap(BOLD, s); }
    inline std::string dim(const std::string& s)     { return wrap(DIM, s); }

}