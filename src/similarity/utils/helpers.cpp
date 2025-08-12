#include <cstdint>
#include <fstream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "helpers.h"

std::queue<SequencePair> read_sequence_file(const std::string &file_path)
{
    std::queue<SequencePair> sequences;
    std::ifstream file(file_path);
    std::string line;
    std::string id;
    std::string sequence;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string t;
        char delimeter = ',';
        std::vector<std::string> sequence_vector;
        while (std::getline(ss, t, delimeter)) {
            sequence_vector.push_back(t);
        }
        sequences.push({std::stoul(sequence_vector[0]), sequence_vector[1], sequence_vector[2]});
    }

    return sequences;
}

std::unordered_map<std::uint64_t, AlignmentResult> process_sequences(std::queue<SequencePair> &sequences)
{
    std::unordered_map<std::uint64_t, AlignmentResult> alignments;
    while (!sequences.empty()) {
        const auto pair = sequences.front();
        sequences.pop();
        const auto id = pair.id;
        const auto seq1 = pair.seq1;
        const auto seq2 = pair.seq2;
        const int gap_penalty = -1;
        const auto alignment = global_alignment(seq1, seq2, gap_penalty);
        alignments[id] = alignment;
    }
    return alignments;
}

void write_aligned_sequence_file(
    const std::unordered_map<std::uint64_t, AlignmentResult> &alignments,
    const std::string &output_file)
{
    std::ofstream file(output_file);
    for (const auto &pair : alignments) {
        const auto id = pair.first;
        const auto alignment = pair.second;
        // << alignment.aligned_seq1 << "," << alignment.aligned_seq2 << ","
        file << id << "," << alignment.similarity_score << std::endl;
    }
}
