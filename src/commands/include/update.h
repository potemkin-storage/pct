// kiwinatra, 2026 (c)

#pragma once

#include "parser.h"

namespace commands {

    // git pull for one packet, or for every packet we know about
    int update(const parser::ParsedCommand& cmd);

}