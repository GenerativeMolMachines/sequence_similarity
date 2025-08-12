#pragma once

#include <queue>
#include <string>
#include <unordered_map>

#include "global_alignment.h"

struct SequencePair {
    std::uint64_t id;
    std::string seq1;
    std::string seq2;
};

std::queue<SequencePair> read_sequence_file(const std::string &file_path);

std::unordered_map<std::uint64_t, AlignmentResult> process_sequences(std::queue<SequencePair> &sequences);

void write_aligned_sequence_file(
    const std::unordered_map<std::uint64_t, AlignmentResult> &alignments,
    const std::string &output_file);
