#include "../catch.hpp"

#include "../nest_msa.h"

TEST_CASE("objective() Test 0", "[objective]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    double o = objective(M, 1);
    CHECK(o == Approx(2.625));
}

TEST_CASE("objective() Test 1", "[objective]") {
    const char *sequences[6];
    sequences[0] = "abcdef";
    sequences[1] = "aaccee";
    sequences[2] = "bbddff";
    sequences[3] = "abaded";
    sequences[4] = "abccdd";
    sequences[5] = "abcdefg";
    Matrix M = create_peer_matrix(6, (char **)sequences);
    double o = objective(M, 1);
    CHECK(o == Approx(6.25));
}
