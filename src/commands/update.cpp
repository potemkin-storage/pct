// kiwinatra, 2026 (c)

#include "update.h"
#include "utils.h"
#include "fs.h"
#include "colors.h"

#include <iostream>
#include <string>

namespace commands {

    // one packet, one git pull, no drama
    static int update_one(const std::string& name, const std::string& dir) {
        std::string dest = fs::path_join(dir, name);

        if (!fs::exists(dest)) {
            utils::err("not installed: " + name);
            return 1;
        }
        if (!fs::is_git_repo(dest)) {
            utils::err("not a git repo: " + dest);
            return 1;
        }

        utils::info("updating " + name);
        if (!utils::run_ok("git -C \"" + dest + "\" pull")) {
            utils::err("git pull failed for " + name);
            return 1;
        }
        utils::ok("updated " + name);
        return 0;
    }

    int update(const parser::ParsedCommand& cmd) {
        std::string base = cmd.output_dir.empty() ? fs::root_dir() : cmd.output_dir;

        if (!cmd.packet.empty()) {
            return update_one(cmd.packet, base);
        }

        // no name = walk everything in .pctc
        std::string cfg_path = parser::pctc_path();
        auto raw = fs::read_file(cfg_path);
        if (!raw) {
            utils::err("no config at " + cfg_path);
            return 1;
        }

        auto cfg = parser::parse_pctc(*raw);
        if (cfg.packets.empty()) {
            utils::warn("nothing installed");
            return 0;
        }

        int failed = 0;
        for (const auto& kv : cfg.packets) {
            if (update_one(kv.first, base) != 0) failed++;
        }

        if (failed) {
            utils::warn(std::to_string(failed) + " packet(s) failed");
            return 1;
        }
        return 0;
    }

}