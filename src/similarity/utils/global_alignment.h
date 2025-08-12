#pragma once

#include <string>
#include <cstdint>

struct AlignmentResult
{
    std::int32_t score;
    double similarity_score;
    std::string aligned_seq1;
    std::string aligned_seq2;
};

double get_match_score(std::string seq1, std::string seq2);

AlignmentResult global_alignment(const std::string& seq1, const std::string& seq2, int gap_penalty);
