#include "../catch.hpp"

#include <string.h>
#include "../nest_msa.h"

TEST_CASE("column() Test 0", "[column]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    char *col = column(M, 0);
    char *correct = new char[8] {'a', 'b', 'c', 'b', 'c', 'd', 'e', 'm'};
    CHECK(memcmp(col, correct, 8) == 0);    
}

TEST_CASE("column() Test 1", "[column]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    char *col = column(M, 1);
    char *correct = new char[8] {'a', 'c', 'b', 'c', 'f', 'g', '#', '#'};
    CHECK(memcmp(col, correct, 8) == 0);    
}
