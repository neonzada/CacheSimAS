#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <random>

int main(int argc, char* argv[]) {
	if(argc == 4 && std::string(argv[1]) == "--gen"){
        int count = std::stoi(argv[2]);
        std::ofstream outputFile(argv[3], std::ios::binary);

        if(!outputFile.is_open()){
            std::cerr << "Error: could not open output file " << argv[3] << "\n";
            return EXIT_FAILURE;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> distrib(0, 0xFFFFFFFF);

        for(int i = 0; i < count; ++i){
            uint32_t addr = distrib(gen);
            outputFile.write(reinterpret_cast<const char*>(&addr), sizeof(addr));
        }

        std::cout << "Generated " << count << " random addresses in binary: " << argv[3] << "\n";
        return EXIT_SUCCESS;

    }else if(argc == 3){
		std::ifstream inputFile(argv[1]);
		std::ofstream outputFile(argv[2], std::ios::binary);

		if(!inputFile.is_open()){
			std::cerr << "Error: could not open input file " << argv[1] << "\n";
			return EXIT_FAILURE;
		}

		if(!outputFile.is_open()){
			std::cerr << "Error: could not open output file " << argv[2] << "\n";
			return EXIT_FAILURE;
		}

		std::string line;
		while(std::getline(inputFile, line)){
			try{
				uint32_t addr = std::stoul(line, nullptr, 16); // base 16
				outputFile.write(reinterpret_cast<const char*>(&addr), sizeof(addr));
			}catch (...){
				std::cerr << "Warning: skipping invalid hex address: '" << line << "'\n";
			}
		}

		std::cout << "Successfully converted to binary: " << argv[2] << "\n";
		return EXIT_SUCCESS;
	}else{
		std::cerr << "Usage:\n";
		std::cerr << "  " << argv[0] << " <hexinput.txt> <output.bin>\n";
		std::cerr << "  " << argv[0] << " --gen <N> <output.bin>\n";
		return EXIT_FAILURE;
	}
}