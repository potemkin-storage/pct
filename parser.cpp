// kiwinatra, 2026 (c)

#include "parser.h"
#include "utils.h"
#include "fs.h"

namespace parser {

    // walk argv once, no second pass, no surprises
    ParsedCommand parse_args(int argc, char** argv) {
        ParsedCommand out;

        if (argc < 2) {
            out.help = true;
            return out;
        }

        std::string first = argv[1];
        if (first == "-h" || first == "--help" || first == "help") {
            out.help = true;
            return out;
        }
        if (first == "-v" || first == "--version" || first == "version") {
            out.version = true;
            return out;
        }

        out.name = first;

        for (int i = 2; i < argc; ++i) {
            std::string a = argv[i];
            if (a == "-o" || a == "--output") {
                if (i + 1 >= argc) {
                    out.rest.push_back(a);
                    continue;
                }
                out.output_dir = argv[++i];
            } else if (a == "-h" || a == "--help") {
                out.help = true;
            } else if (out.packet.empty()) {
                out.packet = a;
            } else {
                out.rest.push_back(a);
            }
        }
        return out;
    }

    // sections matter here, we're not guessing what a key belongs to
    Config parse_pctc(const std::string& content) {
        Config cfg;
        std::string current;
        std::string line;

        auto flush = [&]() {
            std::string t = utils::trim(line);
            line.clear();
            if (t.empty() || t[0] == '#') return;

            if (t.front() == '[' && t.back() == ']') {
                current = t.substr(1, t.size() - 2);
                return;
            }

            std::string::size_type eq = t.find('=');
            if (eq == std::string::npos) return;

            std::string k = utils::trim(t.substr(0, eq));
            std::string v = utils::trim(t.substr(eq + 1));

            if (current == "packets")     cfg.packets[k] = v;
            else if (current == "config") cfg.config[k]  = v;
        };

        for (std::string::size_type i = 0; i < content.size(); ++i) {
            char c = content[i];
            if (c == '\n') flush();
            else line.push_back(c);
        }
        flush();
        return cfg;
    }

    // keys come out sorted (std::map), which is fine for a config file
    std::string serialize_pctc(const Config& cfg) {
        std::string out;
        out += "[packets]\n";
        for (const auto& kv : cfg.packets) out += kv.first + "=" + kv.second + "\n";
        out += "[config]\n";
        for (const auto& kv : cfg.config)  out += kv.first + "=" + kv.second + "\n";
        return out;
    }

    // linux hides it in home, windows owns the whole C:
#ifdef _WIN32
    std::string pctc_path() {
        return fs::path_join(fs::root_dir(), ".pctc");
    }
#else
    std::string pctc_path() {
        return fs::path_join(fs::home_dir(), ".pctc");
    }
#endif

}