#include "../catch.hpp"

#include <string.h>
#include "../nest_msa.h"

TEST_CASE("fly_down() Test 0", "[fly_down]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    Particle p = {
        .value = 'b',
        .pos = {
            .row = 1,
            .num_cols = 3,
            .col = new int[3] {0, 2, 3}
        }
    };
    const char *sequences_correct[4];
    sequences_correct[0] = "a-bcbcdem";
    sequences_correct[1] = "acbcfg";
    sequences_correct[2] = "a-bchimn";
    sequences_correct[3] = "a-bcbcjkm";
    Matrix correct = create_peer_matrix(4, (char **)sequences_correct);
    M = fly_down(p, M);
    CHECK(M.num_rows == correct.num_rows);
    CHECK(M.num_cols == correct.num_cols);
    for (int i = 0; i < correct.num_rows; i++) {
        CHECK(memcmp(M.matrix[i], correct.matrix[i], correct.num_cols) == 0);
    }
}

TEST_CASE("fly_down() Test 1", "[fly_down]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    Particle p = {
        .value = 'b',
        .pos = {
            .row = 1,
            .num_cols = 3,
            .col = new int[3] {0, 2, 3}
        }
    };
    const char *sequences_correct[4];
    sequences_correct[0] = "a---bcbcdem";
    sequences_correct[1] = "acbcfg";
    sequences_correct[2] = "a---bchimn";
    sequences_correct[3] = "a---bcbcjkm";
    Matrix correct = create_peer_matrix(4, (char **)sequences_correct);
    M = fly_down(p, M, 3);
    CHECK(M.num_rows == correct.num_rows);
    CHECK(M.num_cols == correct.num_cols);
    for (int i = 0; i < correct.num_rows; i++) {
        CHECK(memcmp(M.matrix[i], correct.matrix[i], correct.num_cols) == 0);
    }
}
