#include <vector>
#include <string>
#include <algorithm>

#include "global_alignment.h"

int score(char a, char b)
{
    return (a == b) ? 1 : -2; // Simple match/mismatch scoring
}

double get_match_score(std::string seq1, std::string seq2)
{
    auto i = seq1.size(), j = seq2.size();
    int score = 0;
    while (i > 0 && j > 0)
    {
        if (seq1[i] == seq2[j] && seq1[i] != '-')
        {
            score++;
        }
        i--, j--;
    }
    return static_cast<double>(score) / static_cast<double>(seq1.size()) * 100;
}

AlignmentResult global_alignment(const std::string& seq1, const std::string& seq2, int gap_penalty)
{
    size_t m = seq1.size();
    size_t n = seq2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<std::int32_t>(n + 1, 0));

    // Initialize DP table
    for (size_t i = 0; i <= m; ++i) dp[i][0] = i * gap_penalty;
    for (size_t j = 0; j <= n; ++j) dp[0][j] = j * gap_penalty;

    // Fill DP table
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            std::int32_t match = dp[i-1][j-1] + score(seq1[i-1], seq2[j-1]);
            std::int32_t del = dp[i-1][j] + gap_penalty;
            std::int32_t ins = dp[i][j-1] + gap_penalty;
            dp[i][j] = std::max({match, del, ins});
        }
    }

    // Traceback
    std::string align1, align2;
    size_t i = m, j = n;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && dp[i][j] == dp[i-1][j-1] + score(seq1[i-1], seq2[j-1])) {
            align1 = seq1[i-1] + align1;
            align2 = seq2[j-1] + align2;
            --i; --j;
        } else if (i > 0 && dp[i][j] == dp[i-1][j] + gap_penalty) {
            align1 = seq1[i-1] + align1;
            align2 = '-' + align2;
            --i;
        } else {
            align1 = '-' + align1;
            align2 = seq2[j-1] + align2;
            --j;
        }
    }
    double sequence_similarity = get_match_score(align1, align2);
    // std::cout << "Alignment score: " << dp[m][n] << std::endl;
    // std::cout << "Sequence similarity score: " << sequence_similarity << std::endl;
    // std::cout << align1 << std::endl;
    // std::cout << align2 << std::endl;
    // std::cout << std::endl;

    return { dp[m][n], sequence_similarity, align1, align2 };
}
