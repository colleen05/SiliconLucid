// Project: Slew
// File: include/slew.hpp
// Description: Main Slew header.
// License: MIT
// https://github.com/colleen05/Slew

#ifndef SLEW_HPP
#define SLEW_HPP

#define SLEW_VERSION_MAJOR      "0"
#define SLEW_VERSION_MINOR      "0"
#define SLEW_VERSION_RELEASE    "1"

#define SLEW_VERSION "Slew " SLEW_VERSION_MAJOR "." SLEW_VERSION_MINOR
#define SLEW_RELEASE SLEW_VERSION "." SLEW_VERSION_RELEASE

#define SLEW_COPYRIGHT "Copyright (c) 2023 Colleen (\"colleen05\")"
#define SLEW_LICENSE "MIT"

#define SLEW_INFO_LINK "https://github.com/colleen05/Slew"

namespace slew {
    void print_lib_info();
}

#endif