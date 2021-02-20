#include "../catch.hpp"

#include "../nest_msa.h"

TEST_CASE("mostfrequent() Test 0", "[mostfrequent]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    MostFrequent mf = mostfrequent(M.matrix[1]);
    CHECK(mf.freq == 3);
    CHECK(mf.freqChar == 'b');
}

TEST_CASE("mostfrequent() Test 1", "[mostfrequent]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    MostFrequent mf = mostfrequent(M.matrix[2]);
    CHECK(mf.freq == 3);
    CHECK(mf.freqChar == 'c');
}

TEST_CASE("mostfrequent() Test 2", "[mostfrequent]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    MostFrequent mf = mostfrequent(M.matrix[3]);
    CHECK(mf.freq == 2);
    CHECK(mf.freqChar == 'b');
}
