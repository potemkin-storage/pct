// kiwinatra, 2026 (c)

#include "fs.h"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <cstdlib>

#ifdef _WIN32
#else
#include <unistd.h>
#include <pwd.h>
#endif

namespace stdfs = std::filesystem;

namespace fs {

    bool exists(const std::string& path) {
        std::error_code ec;
        return stdfs::exists(path, ec);
    }

    bool is_dir(const std::string& path) {
        std::error_code ec;
        return stdfs::is_directory(path, ec);
    }

    bool is_file(const std::string& path) {
        std::error_code ec;
        return stdfs::is_regular_file(path, ec);
    }

    // no .git = not ours, keep walking
    bool is_git_repo(const std::string& path) {
        return is_dir(path_join(path, ".git"));
    }

    bool make_dir(const std::string& path) {
        std::error_code ec;
        stdfs::create_directories(path, ec);
        return !ec;
    }

    // nukes everything, careful
    bool remove_all(const std::string& path) {
        std::error_code ec;
        stdfs::remove_all(path, ec);
        return !ec;
    }

    std::optional<std::string> read_file(const std::string& path) {
        std::ifstream f(path, std::ios::binary);
        if (!f) return std::nullopt;
        std::string content((std::istreambuf_iterator<char>(f)),
                             std::istreambuf_iterator<char>());
        return content;
    }

    bool write_file(const std::string& path, const std::string& content) {
        std::ofstream f(path, std::ios::binary | std::ios::trunc);
        if (!f) return false;
        f.write(content.data(), content.size());
        return f.good();
    }

    std::string home_dir() {
#ifdef _WIN32
        const char* h = std::getenv("USERPROFILE");
        if (h && *h) return h;
        h = std::getenv("HOMEDRIVE");
        const char* p = std::getenv("HOMEPATH");
        if (h && p) return std::string(h) + p;
        return "";
#else
        const char* h = std::getenv("HOME");
        if (h && *h) return h;
        struct passwd* pw = getpwuid(getuid());
        if (pw && pw->pw_dir) return pw->pw_dir;
        return "";
#endif
    }

    // windows folks live on C:, we live on /
    std::string root_dir() {
#ifdef _WIN32
        return "C:\\";
#else
        return "/";
#endif
    }

    // glue two paths without doubling the separator
    std::string path_join(const std::string& a, const std::string& b) {
        if (a.empty()) return b;
        if (b.empty()) return a;
#ifdef _WIN32
        const char sep = '\\';
        const bool a_end = (a.back() == '/' || a.back() == '\\');
        const bool b_start = (b.front() == '/' || b.front() == '\\');
#else
        const char sep = '/';
        const bool a_end = (a.back() == '/');
        const bool b_start = (b.front() == '/');
#endif
        if (a_end && b_start) return a + b.substr(1);
        if (a_end || b_start) return a + b;
        return a + sep + b;
    }

    // just the dir names, files get ignored
    std::vector<std::string> list_dirs(const std::string& path) {
        std::vector<std::string> out;
        std::error_code ec;
        if (!stdfs::is_directory(path, ec)) return out;
        for (auto& e : stdfs::directory_iterator(path, ec)) {
            std::error_code ec2;
            if (e.is_directory(ec2)) out.push_back(e.path().filename().string());
        }
        return out;
    }

}