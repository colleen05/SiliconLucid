// Project: Slew
// File: src/slew.cpp
// Description: Main Slew source file.
// License: MIT
// https://github.com/colleen05/Slew

#include <slew/slew.hpp>

#include <iostream>

void slew::print_lib_info() {
    std::cout
        << "Using " SLEW_RELEASE " " SLEW_COPYRIGHT "." << std::endl
        << "Distributed under the " SLEW_LICENSE " license." << std::endl
        << "Dependencies may be distributed under different licenses." << std::endl
        << std::endl
        << "See " SLEW_INFO_LINK " for more details." << std::endl;
}