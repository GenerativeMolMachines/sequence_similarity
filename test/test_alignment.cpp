#include <string>
#include <catch2/catch_test_macros.hpp>

#include "similarity/utils/global_alignment.h"

TEST_CASE("match_score_simple", "[global_alignment_helper]")
{
    std::string seq1 = "GCA-TGCG";
    std::string seq2 = "G-ATTACA";
    double test_match_score = 4.0 / 8.0 * 100;
    double match_score = get_match_score(seq1, seq2);
    REQUIRE((match_score - test_match_score) < 0.01);
}

TEST_CASE("global_alignment_simple", "[global_alignment]") {
    std::string seq1 = "GCATGCG";
    std::string seq2 = "GATTACA";

    std::string aligned_seq_1 = "GCA-TGCG";
    std::string aligned_seq_2 = "G-ATTACA";

    int gap_penalty = -1;
    AlignmentResult result = global_alignment(seq1, seq2, gap_penalty);

    REQUIRE(result.aligned_seq1 == aligned_seq_1);
    REQUIRE(result.aligned_seq2 == aligned_seq_2);
}

TEST_CASE("global_alignment_basic", "[global_alignment]") {
    std::string seq1 = "MSSSSPSSQSRLSVPGRTPHLPPLTIPHTVSAEGLATPNTPHALLPTG";
    std::string seq2 = "PHRYRPGTVALREIRRYQKSTELLIRKLPFQRLVREIAQDFKTDLRFQSSAVMALQEACEAYLVGLFEDTNLCAIHAKRVTI";

    std::string aligned_seq_1 = "MSSSSPSSQSRLSVPG--------R----T----PHLP--PL--TI---PHT--------V----SA-E----GL--ATPN--TPHALLPTG";
    std::string aligned_seq_2 = "-----P---HR-YRPGTVALREIRRYQKSTELLIRKLPFQRLVREIAQDFKTDLRFQSSAVMALQEACEAYLVGLFEDT-NLCAIHAKRVTI";

    [[maybe_unused]] float score = 21.0;

    int gap_penalty = -1;
    AlignmentResult result = global_alignment(seq1, seq2, gap_penalty);

    REQUIRE(result.aligned_seq1 == aligned_seq_1);
    REQUIRE(result.aligned_seq2 == aligned_seq_2);
}
