#include "../catch.hpp"

#include <string.h>
#include "../nest_msa.h"

TEST_CASE("row_alignment() Test 0", "[row_alignment]") {
    const char *sequences[5];
    sequences[0] = "abbc";
    sequences[1] = "abcd";
    sequences[2] = "bbcd";
    sequences[3] = "cbdd";
    sequences[4] = "dbde";
    Matrix M = create_peer_matrix(5, (char **)sequences);
    Particle p = row_alignment(1, M);
    CHECK(p.value == -1);
}

TEST_CASE("row_alignment() Test 1", "[row_alignment]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    Particle correct = {
        .value = 'c',
        .pos = {
            .row = 1,
            .num_cols = 1,
            .col = new int[1] {1}
        },
        .best = {
            .row = 2,
            .num_cols = 4,
            .col = new int[4] {0, 1, 2, 3}
        },
        .best_value = 9.0
    };
    Particle p = row_alignment(1, M);
    CHECK(p.value == correct.value);
    CHECK(p.best_value == correct.best_value);
    CHECK(p.pos.row == correct.pos.row);
    CHECK(p.pos.num_cols == correct.pos.num_cols);
    CHECK(memcpy(p.pos.col, correct.pos.col, correct.pos.num_cols) == 0);
    CHECK(p.best.row == correct.best.row);
    CHECK(p.best.num_cols == correct.best.num_cols);
    CHECK(memcpy(p.best.col, correct.best.col, correct.pos.num_cols) == 0);
}

TEST_CASE("row_alignment() Test 2", "[row_alignment]") {
    const char *sequences[4];
    sequences[0] = "abcd";
    sequences[1] = "abcd";
    sequences[2] = "-bcd";
    sequences[3] = "abcd";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    Particle p = row_alignment(0, M);
    CHECK(p.value == -1);
}
