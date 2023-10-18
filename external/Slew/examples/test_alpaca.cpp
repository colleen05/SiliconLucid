// Project: Slew
// File: examples/test_alpaca.cpp
// Description: TEST example for Alpaca serialisation.
// License: MIT
// https://github.com/colleen05/Slew

#include <slew/slew.hpp>
#include <alpaca/alpaca.h>

#include <iostream>
#include <vector>
#include <cstdint>

struct Config {
    std::string foo;
    std::vector<int> bar;
    uint16_t baz;
};

int main() {
    slew::print_lib_info();
    std::cout << std::endl;

    std::vector<uint8_t> out_bytes;
    std::vector<uint8_t> in_bytes;

    Config my_struct = {
        "Hello, world!",
        {1, 2, 3, 4, 5},
        1234
    };

    // Serialise
    std::cout << "Serialising data... ";
    try {
        alpaca::serialize(my_struct, out_bytes);
        std::cout << "Done." << std::endl;
    } catch(const std::exception &e) {
        std::cout << "\nFAILED: " << e.what() << std::endl;
    }

    // Write to file
    std::cout << "Writing to file... ";
    try {
        std::ofstream f("alpaca_out.bin", std::ios::binary);
        f.write((char*)&out_bytes[0], out_bytes.size());
        f.close();
        std::cout << "Done." << std::endl;
    } catch(const std::exception &e) {
        std::cout << "\nFAILED: " << e.what() << std::endl;
        return 1;
    }

    // Read from file
    std::cout << "Reading from file... ";
    try {
        std::ifstream f("alpaca_out.bin", std::ios::binary);
        in_bytes = std::vector<uint8_t>(
            std::istreambuf_iterator<char>(f),
            std::istreambuf_iterator<char>()
        );
        f.close();
        std::cout << "Done." << std::endl;
    } catch(const std::exception &e) {
        std::cout << "\nFAILED: " << e.what() << std::endl;
        return 1;
    }

    // Deserialise
    std::cout << "Deserialising data... ";
    std::error_code ec;
    auto object = alpaca::deserialize<Config>(in_bytes, ec);
    if (!ec) {
        std::cout << "Success." << std::endl;
    }else {
        std::cout << "\nFAILED: " << ec.message() << std::endl;
        return 1;
    }

    // Print results
    std::cout
        << "\n===== DESERIALISED STRUCT =====" << std::endl
        << "Config object = {" << std::endl
        << "    std::string foo = \"" << object.foo << "\";" << std::endl
        << "    std::vector<int> bar = [";

    for(int i = 0; i < object.bar.size(); i++) {
        std::cout << object.bar[i];
        if(i < object.bar.size() - 1)
            std::cout << ", ";
    }

    std::cout << "];" << std::endl;

    std::cout
        << "    uint16_t baz = " << object.baz << ";" << std::endl
        << "}" << std::endl
        << std::endl;

    return 0;
}