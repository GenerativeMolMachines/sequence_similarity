#include <chrono>
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

    auto start_time = std::chrono::system_clock::now();
    std::time_t start_time_c = std::chrono::system_clock::to_time_t(start_time);

    std::cout << "Start time: " << std::ctime(&start_time_c);

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
    std::uint64_t batch_counter = 0;

    std::uint64_t line_counter = 0;
    while (std::getline(file, line)) {
        if (line_counter == 0) {
            line_counter++;
            continue;
        }
        std::stringstream ss(line);
        std::string t;
        char delimeter = ',';
        std::vector<std::string> sequence_vector;
        while (std::getline(ss, t, delimeter)) {
            sequence_vector.push_back(t);
        }
        sequences.push({std::stoul(sequence_vector[0]), sequence_vector[1], sequence_vector[2]});
        line_counter++;

        if (sequences.size() == batch_size) {
            auto batch_start_time = std::chrono::system_clock::now();

            auto results = process_sequences(sequences);
            write_aligned_sequence_file(results, output_file);

            batch_counter++;

            auto batch_end_time = std::chrono::system_clock::now();
            std::time_t batch_end_time_c = std::chrono::system_clock::to_time_t(batch_end_time);
            std::cout << "Batch completed at " << std::ctime(&batch_end_time_c);

            std::cout << "Time taken on batch: " << std::chrono::duration_cast<std::chrono::seconds>(batch_end_time - batch_start_time).count() << " ms" << std::endl;
            std::cout << "Batches completed: " << batch_counter << std::endl;
        }
    }

    auto results = process_sequences(sequences);
    write_aligned_sequence_file(results, output_file);

    auto end_time = std::chrono::system_clock::now();
    std::time_t end_time_c = std::chrono::system_clock::to_time_t(end_time);
    std::cout << "Total time taken: " << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << " seconds" << std::endl;
    std::cout << "End time: " << std::ctime(&end_time_c) << std::endl;
}
