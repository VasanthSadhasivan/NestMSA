#include "../catch.hpp"

#include <string.h>
#include "../nest_msa.h"

TEST_CASE("getposition() Test 0", "[getposition]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    Particle correct = {
        .value = 'b',
        .pos = {
            .row = 1,
            .num_cols = 3,
            .col = new int[3] {0, 2, 3}
        }
    };
    Particle p = getposition('b', 1, M);
    CHECK(p.value == correct.value);
    CHECK(p.pos.row == correct.pos.row);
    CHECK(p.pos.num_cols == correct.pos.num_cols);
    CHECK(memcmp(p.pos.col, correct.pos.col, correct.pos.num_cols) == 0);
}

TEST_CASE("getposition() Test 1", "[getposition]") {
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
        }
    };
    Particle p = getposition('c', 1, M);
    CHECK(p.value == correct.value);
    CHECK(p.pos.row == correct.pos.row);
    CHECK(p.pos.num_cols == correct.pos.num_cols);
    CHECK(memcmp(p.pos.col, correct.pos.col, correct.pos.num_cols) == 0);
}

TEST_CASE("getposition() Test 2", "[getposition]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    Particle correct = {
        .value = 'b',
        .pos = {
            .row = 0,
            .num_cols = 0,
            .col = new int[0] {}
        }
    };
    Particle p = getposition('c', 1, M);
    CHECK(p.value == correct.value);
    CHECK(p.pos.row == correct.pos.row);
    CHECK(p.pos.num_cols == correct.pos.num_cols);
    CHECK(memcmp(p.pos.col, correct.pos.col, correct.pos.num_cols) == 0);
}
