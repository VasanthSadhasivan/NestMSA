#include "../catch.hpp"

#include <string.h>
#include "../nest_msa.h"

TEST_CASE("remove_missing_rows() Test 0", "[remove_missing_rows]") {
    const char *sequences[4];
    sequences[0] = "abcbcde";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M1 = create_peer_matrix(4, (char **)sequences);
    Matrix correct = create_peer_matrix(4, (char **)sequences);
    Matrix M = remove_missing_rows(M1);
    CHECK(M.num_rows == correct.num_rows);
    CHECK(M.num_cols == correct.num_cols);
    for (int i = 0; i < correct.num_rows; i++) {
        CHECK(memcmp(M.matrix[i], correct.matrix[i], correct.num_cols) == 0);
    }
}

TEST_CASE("remove_missing_rows() Test 1", "[remove_missing_rows]") {
    const char *sequences[4];
    sequences[0] = "abcbcde";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm-";
    const char *sequences_correct[4];
    sequences_correct[0] = "abcbcde";
    sequences_correct[1] = "acbcfg";
    sequences_correct[2] = "abchimn";
    sequences_correct[3] = "abcbcjkm";
    Matrix M1 = create_peer_matrix(4, (char **)sequences);
    Matrix correct = create_peer_matrix(4, (char **)sequences_correct);
    Matrix M = remove_missing_rows(M1);
    CHECK(M.num_rows == correct.num_rows);
    CHECK(M.num_cols == correct.num_cols);
    for (int i = 0; i < correct.num_rows; i++) {
        CHECK(memcmp(M.matrix[i], correct.matrix[i], correct.num_cols) == 0);
    }
}

TEST_CASE("remove_missing_rows() Test 2", "[remove_missing_rows]") {
    const char *sequences[4];
    sequences[0] = "abcbcde";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn---";
    sequences[3] = "abcbcjkm--";
    const char *sequences_correct[4];
    sequences_correct[0] = "abcbcde";
    sequences_correct[1] = "acbcfg";
    sequences_correct[2] = "abchimn-";
    sequences_correct[3] = "abcbcjkm";
    Matrix M1 = create_peer_matrix(4, (char **)sequences);
    Matrix correct = create_peer_matrix(4, (char **)sequences_correct);
    Matrix M = remove_missing_rows(M1);
    CHECK(M.num_rows == correct.num_rows);
    CHECK(M.num_cols == correct.num_cols);
    for (int i = 0; i < correct.num_rows; i++) {
        CHECK(memcmp(M.matrix[i], correct.matrix[i], correct.num_cols) == 0);
    }
}
