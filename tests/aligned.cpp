#include "../catch.hpp"

#include "../nest_msa.h"

TEST_CASE("aligned() Test 0", "[aligned]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    CHECK(aligned(M.matrix[0], M.num_cols));    
}

TEST_CASE("aligned() Test 1", "[aligned]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    CHECK(aligned(M.matrix[7], M.num_cols));    
}

TEST_CASE("aligned() Test 2", "[aligned]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    CHECK_FALSE(aligned(M.matrix[6], M.num_cols));    
}
