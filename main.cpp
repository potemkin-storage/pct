// kiwinatra, 2026 (c)

#include "parser.h"
#include "utils.h"
#include "colors.h"

#include "pull.h"
#include "update.h"
#include "remove.h"

#include <iostream>
#include <string>

static const char* PCT_VERSION = "0.1.3";

static void print_help() {
    std::cout
        << colors::bold("pct") << " - potemkin package manager\n\n"
        << colors::bold("usage:\n")
        << "  pct pull <packet> [-o <dir>]    download and register a packet\n"
        << "  pct update [packet] [-o <dir>]  refresh one packet, or all of them\n"
        << "  pct remove <packet> [-o <dir>]  wipe a packet and forget it\n"
        << "  pct help                        this text\n"
        << "  pct version                     print version\n\n"
        << colors::bold("options:\n")
        << "  -o, --output <dir>   install somewhere other than the disk root\n"
        << "  -h, --help           same as 'pct help'\n"
        << "  -v, --version        same as 'pct version'\n";
}

// one place that knows the command names, so main stays dumb
static int dispatch(const parser::ParsedCommand& cmd) {
    if (cmd.name == "pull")   return commands::pull(cmd);
    if (cmd.name == "update") return commands::update(cmd);
    if (cmd.name == "remove") return commands::remove(cmd);

    utils::err("unknown command: " + cmd.name);
    std::cout << "run " << colors::bold("pct help") << " to see what's there\n";
    return 1;
}

int main(int argc, char** argv) {
    parser::ParsedCommand cmd = parser::parse_args(argc, argv);

    if (cmd.version) {
        std::cout << "pct " << PCT_VERSION << "\n";
        return 0;
    }
    if (cmd.help || cmd.name.empty()) {
        print_help();
        return 0;
    }

    // -o without a value lands here, catch it before it does damage
    for (const auto& a : cmd.rest) {
        if (a == "-o" || a == "--output") {
            utils::err("missing value for " + a);
            return 1;
        }
    }
    if (!cmd.rest.empty()) {
        utils::err("unexpected argument: " + cmd.rest.front());
        return 1;
    }

    return dispatch(cmd);
}