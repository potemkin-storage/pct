// kiwinatra, 2026 (c)

#include "remove.h"
#include "utils.h"
#include "fs.h"
#include "colors.h"

#include <iostream>
#include <string>

namespace commands {

    // strike the packet from the global config
    static bool unregister_packet(const std::string& name) {
        std::string path = parser::pctc_path();
        auto raw = fs::read_file(path);
        if (!raw) return true; // nothing to clean, not our problem

        auto cfg = parser::parse_pctc(*raw);
        cfg.packets.erase(name);
        return fs::write_file(path, parser::serialize_pctc(cfg));
    }

    int remove(const parser::ParsedCommand& cmd) {
        if (cmd.packet.empty()) {
            utils::err("usage: pct remove <packet> [-o <dir>]");
            return 1;
        }

        std::string base = cmd.output_dir.empty() ? fs::root_dir() : cmd.output_dir;
        std::string dest = fs::path_join(base, cmd.packet);

        if (!fs::exists(dest)) {
            utils::err("not installed: " + cmd.packet);
            return 1;
        }

        // deleting stuff from root, better ask first
        std::cout << "about to delete " << colors::bold(dest) << "\n";
        if (!utils::confirm("sure?")) {
            utils::info("aborted");
            return 0;
        }

        if (!fs::remove_all(dest)) {
            utils::err("couldn't remove " + dest);
            return 1;
        }

        if (!unregister_packet(cmd.packet)) {
            utils::warn("couldn't update " + parser::pctc_path());
        }

        utils::ok("removed " + cmd.packet);
        return 0;
    }

}