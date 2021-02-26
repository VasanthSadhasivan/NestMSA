#include "../catch.hpp"

#include <string.h>
#include "../nest_msa.h"

TEST_CASE("nest_msa_main() Test 0", "[nest_msa_main]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    M = nest_msa_main(M);
    int rows = M.num_rows;
    int cols = M.num_cols;
    CHECK(rows == 9);
    CHECK(cols == 4);
    char correct[9][4] = {
        {'a', 'a', 'a', 'a'},
        {'b', '-', 'b', 'b'},
        {'c', 'c', 'c', 'c'},
        {'b', 'b', '-', 'b'},
        {'c', 'c', '-', 'c'},
        {'d', 'f', 'h', 'j'},
        {'e', 'g', 'i', 'k'},
        {'m', '#', 'm', 'm'},
        {'#', '#', 'n', '#'}
    };
    for (int i = 0; i < rows; i++) {
        CHECK(memcmp(M.matrix[i], correct[i], cols) == 0);
    }
}

TEST_CASE("nest_msa_main() Test 1", "[nest_msa_main]") {
    const char *sequences[5];
    sequences[0] = "abbccdd";
    sequences[1] = "abccdd";
    sequences[2] = "abcdd";
    sequences[3] = "aabccdd";
    sequences[4] = "aabccc";
    Matrix M = create_peer_matrix(5, (char **)sequences);
    M = nest_msa_main(M);
    int rows = M.num_rows;
    int cols = M.num_cols;
    CHECK(rows == 12);
    CHECK(cols == 5);
    char correct[12][5] = {
        {'a', 'a', 'a', 'a', 'a'},
        {'-', '-', '-', 'a', 'a'},
        {'b', 'b', 'b', 'b', 'b'},
        {'-', 'c', 'c', 'c', 'c'},
        {'-', 'c', 'd', 'c', 'c'},
        {'-', 'd', 'd', 'd', 'c'},
        {'-', 'd', '#', 'd', '#'},
        {'b', '#', '#', '#', '#'},
        {'c', '#', '#', '#', '#'},
        {'c', '#', '#', '#', '#'},
        {'d', '#', '#', '#', '#'},
        {'d', '#', '#', '#', '#'}
    };
    for (int i = 0; i < rows; i++) {
        CHECK(memcmp(M.matrix[i], correct[i], cols) == 0);
    }
}

TEST_CASE("nest_msa_main() Test 2", "[nest_msa_main]") {
    const char *sequences[6];
    sequences[0] = "abcdd";
    sequences[1] = "abcd";
    sequences[2] = "abcc";
    sequences[3] = "abdd";
    sequences[4] = "abcc";
    sequences[5] = "add";
    Matrix M = create_peer_matrix(6, (char **)sequences);
    M = nest_msa_main(M);
    int rows = M.num_rows;
    int cols = M.num_cols;
    CHECK(rows == 5);
    CHECK(cols == 6);
    char correct[5][6] = {
        {'a', 'a', 'a', 'a', 'a', 'a'},
        {'b', 'b', 'b', 'b', 'b', '-'},
        {'c', 'c', 'c', 'd', 'c', 'd'},
        {'d', 'd', 'c', 'd', 'c', 'd'},
        {'d', '#', '#', '#', '#', '#'}
    };
    for (int i = 0; i < rows; i++) {
        CHECK(memcmp(M.matrix[i], correct[i], cols) == 0);
    }
}

TEST_CASE("nest_msa_main() Test 3", "[nest_msa_main]") {
    const char *sequences[2];
    sequences[0] = "abcdd";
    sequences[1] = "abdd";
    Matrix M = create_peer_matrix(2, (char **)sequences);
    M = nest_msa_main(M);
    int rows = M.num_rows;
    int cols = M.num_cols;
    CHECK(rows == 5);
    CHECK(cols == 2);
    char correct[5][2] = {
        {'a', 'a'},
        {'b', 'b'},
        {'c', '-'},
        {'d', 'd'},
        {'d', 'd'}
    };
    for (int i = 0; i < rows; i++) {
        CHECK(memcmp(M.matrix[i], correct[i], cols) == 0);
    }
}

TEST_CASE("nest_msa_main() Test 4", "[nest_msa_main]") {
    const char *sequences[2];
    sequences[0] = "aaabbbcccddd";
    sequences[1] = "aaaccc";
    Matrix M = create_peer_matrix(2, (char **)sequences);
    M = nest_msa_main(M);
    int rows = M.num_rows;
    int cols = M.num_cols;
    CHECK(rows == 12);
    CHECK(cols == 2);
    char correct[12][2] = {
        {'a', 'a'},
        {'a', 'a'},
        {'a', 'a'},
        {'b', '-'},
        {'b', '-'},
        {'b', '-'},
        {'c', 'c'},
        {'c', 'c'},
        {'c', 'c'},
        {'d', '#'},
        {'d', '#'},
        {'d', '#'}
    };
    for (int i = 0; i < rows; i++) {
        CHECK(memcmp(M.matrix[i], correct[i], cols) == 0);
    }
}

TEST_CASE("nest_msa_main() Test 5", "[nest_msa_main]") {
    const char *sequences[2];
    sequences[0] = "abbcdcba";
    sequences[1] = "abcdcbba";
    Matrix M = create_peer_matrix(2, (char **)sequences);
    M = nest_msa_main(M);
    int rows = M.num_rows;
    int cols = M.num_cols;
    CHECK(rows == 9);
    CHECK(cols == 2);
    char correct[9][2] = {
        {'a', 'a'},
        {'b', 'b'},
        {'b', '-'},
        {'c', 'c'},
        {'d', 'd'},
        {'c', 'c'},
        {'b', 'b'},
        {'-', 'b'},
        {'a', 'a'}
    };
    for (int i = 0; i < rows; i++) {
        CHECK(memcmp(M.matrix[i], correct[i], cols) == 0);
    }
}
