#include "../catch.hpp"

#include <string.h>
#include "../nest_msa.h"

TEST_CASE("create_peer_matrix() Test 0", "[create_peer_matrix]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    int rows = M.num_rows;
    int cols = M.num_cols;
    CHECK(rows == 8);
    CHECK(cols == 4);
    char correct[8][4] = {
        {'a', 'a', 'a', 'a'},
        {'b', 'c', 'b', 'b'},
        {'c', 'b', 'c', 'c'},
        {'b', 'c', 'h', 'b'},
        {'c', 'f', 'i', 'c'},
        {'d', 'g', 'm', 'j'},
        {'e', '#', 'n', 'k'},
        {'m', '#', '#', 'm'}
    };
    for (int i = 0; i < rows; i++) {
        CHECK(memcmp(M.matrix[i], correct[i], cols) == 0);
    }
}

TEST_CASE("create_peer_matrix() Test 1", "[create_peer_matrix]") {
    const char *sequences[5];
    sequences[0] = "abbc";
    sequences[1] = "abcd";
    sequences[2] = "bbcd";
    sequences[3] = "cddd";
    sequences[4] = "ddde";
    Matrix M = create_peer_matrix(5, (char **)sequences);
    int rows = M.num_rows;
    int cols = M.num_cols;
    CHECK(rows == 4);
    CHECK(cols == 5);
    char correct[4][5] = {
        {'a', 'a', 'b', 'c', 'd'},
        {'b', 'b', 'b', 'd', 'd'},
        {'b', 'c', 'c', 'd', 'd'},
        {'c', 'd', 'd', 'd', 'e'}
    };
    for (int i = 0; i < rows; i++) {
        CHECK(memcmp(M.matrix[i], correct[i], cols) == 0);
    }
}


