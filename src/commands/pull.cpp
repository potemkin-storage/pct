// kiwinatra, 2026 (c)

#include "pull.h"
#include "utils.h"
#include "fs.h"
#include "colors.h"

#include <filesystem>
#include <iostream>
#include <string>

namespace stdfs = std::filesystem;

namespace commands {

    static const char* REPO_BASE = "https://github.com/potemkin-storage/";

    // the repo ships exactly one .pcmetadata, we just find it
    static std::string find_metadata(const std::string& dir) {
        std::error_code ec;
        if (!stdfs::is_directory(dir, ec)) return "";
        for (auto& e : stdfs::directory_iterator(dir, ec)) {
            std::error_code ec2;
            if (!e.is_regular_file(ec2)) continue;
            std::string name = e.path().filename().string();
            if (utils::ends_with(name, ".pcmetadata")) return e.path().string();
        }
        return "";
    }

    // remember the packet in the global config
    static bool register_packet(const std::string& name, const std::string& version) {
        std::string path = parser::pctc_path();
        parser::Config cfg;
        auto raw = fs::read_file(path);
        if (raw) cfg = parser::parse_pctc(*raw);
        cfg.packets[name] = version.empty() ? "?" : version;
        return fs::write_file(path, parser::serialize_pctc(cfg));
    }

    int pull(const parser::ParsedCommand& cmd) {
        if (cmd.packet.empty()) {
            utils::err("usage: pct pull <packet> [-o <dir>]");
            return 1;
        }

        std::string base = cmd.output_dir.empty() ? fs::root_dir() : cmd.output_dir;
        std::string dest = fs::path_join(base, cmd.packet);

        if (fs::exists(dest)) {
            if (fs::is_git_repo(dest)) {
                utils::err("'" + cmd.packet + "' is already installed at " + dest);
                utils::info("to refresh it: pct update " + cmd.packet);
            } else {
                utils::err("directory already exists: " + dest);
                utils::info("pick another spot: pct pull " + cmd.packet + " -o <dir>");
            }
            return 1;
        }

        std::string url = std::string(REPO_BASE) + cmd.packet;
        utils::info("cloning " + url);
        if (!utils::run_ok("git clone " + url + " \"" + dest + "\"")) {
            utils::err("git clone failed");
            return 1;
        }

        std::string meta_path = find_metadata(dest);
        if (meta_path.empty()) {
            utils::warn("no .pcmetadata in " + cmd.packet);
            utils::ok("installed to " + dest);
            return 0;
        }

        auto raw = fs::read_file(meta_path);
        if (!raw) {
            utils::err("couldn't read " + meta_path);
            return 1;
        }

        auto meta = utils::parse_kv(*raw);
        std::string name    = meta.count("name")    ? meta["name"]    : cmd.packet;
        std::string version = meta.count("version") ? meta["version"] : "";
        std::string author  = meta.count("author")  ? meta["author"]  : "";
        std::string type    = meta.count("type")    ? meta["type"]    : "file";

        std::cout << colors::bold(name);
        if (!version.empty()) std::cout << " " << colors::dim(version);
        std::cout << "\n";
        if (!author.empty()) std::cout << "by " << author << "\n";

        if (!register_packet(cmd.packet, version)) {
            utils::warn("couldn't write " + parser::pctc_path());
        }

        if (type == "exc") {
#ifdef _WIN32
            std::string install = meta.count("install_cmd_wn") ? meta["install_cmd_wn"] : "";
#else
            std::string install = meta.count("install_cmd_linux") ? meta["install_cmd_linux"] : "";
#endif
            if (install.empty()) {
                utils::warn("no install command for this platform");
            } else {
                utils::info("cd " + dest + " && " + install);
            }
        } else {
            utils::ok("installed to " + dest);
        }

        return 0;
    }

}