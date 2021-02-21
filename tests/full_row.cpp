#include "../catch.hpp"

#include "../nest_msa.h"

TEST_CASE("full_row() Test 0", "[full_row]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    CHECK(full_row(M.matrix[0], M.num_cols));
}

TEST_CASE("full_row() Test 1", "[full_row]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    CHECK_FALSE(full_row(M.matrix[1], M.num_cols));
}

TEST_CASE("full_row() Test 2", "[full_row]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    CHECK_FALSE(full_row(M.matrix[2], M.num_cols));
}

TEST_CASE("full_row() Test 3", "[full_row]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    CHECK_FALSE(full_row(M.matrix[3], M.num_cols));
}
