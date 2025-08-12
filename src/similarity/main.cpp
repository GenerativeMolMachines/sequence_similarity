#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils/helpers.h"


int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
if (argc < 2) {
        std::cerr << "Error: Missing required arguments." << std::endl;
        std::cerr << "Usage: " << argv[0] << " --input <input_file> --output <output_file>" << std::endl;
        std::cerr << "       " << argv[0] << " --help" << std::endl;
        return 1;
    }

    std::string input_file;
    std::string output_file;
    bool input_set = false;
    bool output_set = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " --input <input_file> --output <output_file>" << std::endl;
            std::cout << "\nOptions:\n";
            std::cout << "  --input <file>   Input file path (mandatory)" << std::endl;
            std::cout << "  --output <file>  Output file path (mandatory)" << std::endl;
            std::cout << "  --help           Show this help message" << std::endl;
            return 0;
        }
        else if (arg == "--input") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --input requires a file path argument." << std::endl;
                return 1;
            }
            input_file = argv[++i];
            input_set = true;
        }
        else if (arg == "--output") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --output requires a file path argument." << std::endl;
                return 1;
            }
            output_file = argv[++i];
            output_set = true;
        }
        else {
            std::cerr << "Error: Unknown argument '" << arg << "'." << std::endl;
            std::cerr << "Use --help for usage information." << std::endl;
            return 1;
        }
    }

    if (!input_set) {
        std::cerr << "Error: --input argument is mandatory." << std::endl;
        return 1;
    }

    if (!output_set) {
        std::cerr << "Error: --output argument is mandatory." << std::endl;
        return 1;
    }

    std::queue<SequencePair> sequences;
    std::ifstream file(input_file);
    std::string line;
    std::string id;
    std::string sequence;

    std::uint16_t batch_size = 1024;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string t;
        char delimeter = ',';
        std::vector<std::string> sequence_vector;
        while (std::getline(ss, t, delimeter)) {
            sequence_vector.push_back(t);
        }
        sequences.push({std::stoul(sequence_vector[0]), sequence_vector[1], sequence_vector[2]});

        if (sequences.size() == batch_size) {
            auto results = process_sequences(sequences);
            write_aligned_sequence_file(results, output_file);
        }
    }

    auto results = process_sequences(sequences);
    write_aligned_sequence_file(results, output_file);
}
