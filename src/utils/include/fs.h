// kiwinatra, 2026 (c)

#pragma once

#include <string>
#include <vector>
#include <optional>

namespace fs {

    // existence checks, obviously
    bool exists(const std::string& path);
    bool is_dir(const std::string& path);
    bool is_file(const std::string& path);
    bool is_git_repo(const std::string& path);

    bool make_dir(const std::string& path);
    bool remove_all(const std::string& path);

    std::optional<std::string> read_file(const std::string& path);
    bool write_file(const std::string& path, const std::string& content);

    std::string home_dir();
    std::string root_dir();
    std::string path_join(const std::string& a, const std::string& b);

    std::vector<std::string> list_dirs(const std::string& path);

}