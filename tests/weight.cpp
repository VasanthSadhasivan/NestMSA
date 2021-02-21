#include "../catch.hpp"

#include "../nest_msa.h"

TEST_CASE("weight() Test 0", "[weight]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    double w = weight(M.matrix[0], M.num_cols);
    CHECK(w == Approx(1.0));
}

TEST_CASE("weight() Test 1", "[weight]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    double w = weight(M.matrix[1], M.num_cols);
    CHECK(w == Approx(0.1875));
}
