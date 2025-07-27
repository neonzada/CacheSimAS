#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_hex.txt> <output_bin.bin>\n";
        return EXIT_FAILURE;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2], std::ios::binary);

    if (!inputFile.is_open()) {
        std::cerr << "Error: could not open input file " << argv[1] << "\n";
        return EXIT_FAILURE;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Error: could not open output file " << argv[2] << "\n";
        return EXIT_FAILURE;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        try {
            uint32_t addr = std::stoul(line, nullptr, 16); // base 16
            outputFile.write(reinterpret_cast<const char*>(&addr), sizeof(addr));
        } catch (...) {
            std::cerr << "Warning: skipping invalid hex address: '" << line << "'\n";
        }
    }

    std::cout << "Successfully converted to binary: " << argv[2] << "\n";
    return EXIT_SUCCESS;
}