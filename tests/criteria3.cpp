#include "../catch.hpp"

#include "../nest_msa.h"

TEST_CASE("criteria3() Test 0", "[criteria3]") {
    const char *sequences[5];
    sequences[0] = "abbc";
    sequences[1] = "abcd";
    sequences[2] = "bbcd";
    sequences[3] = "cddd";
    sequences[4] = "ddde";
    Matrix M = create_peer_matrix(5, (char **)sequences);
    Particle p = {
        .value = 'b',
        .pos = {
            .row = 0,
            .num_cols = 1,
            .col = new int[1] {2}
        }
    };
    CHECK(criteria3(p, 1, M));
}

TEST_CASE("criteria3() Test 1", "[criteria3]") {
    const char *sequences[5];
    sequences[0] = "abbc";
    sequences[1] = "abcd";
    sequences[2] = "bbcd";
    sequences[3] = "cddd";
    sequences[4] = "ddde";
    Matrix M = create_peer_matrix(5, (char **)sequences);
    Particle p = {
        .value = 'b',
        .pos = {
            .row = 0,
            .num_cols = 3,
            .col = new int[3] {0, 1, 2}
        }
    };
    CHECK_FALSE(criteria3(p, 1, M));
}

TEST_CASE("criteria3() Test 2", "[criteria3]") {
    const char *sequences[5];
    sequences[0] = "abbc";
    sequences[1] = "abcd";
    sequences[2] = "bbcd";
    sequences[3] = "cddd";
    sequences[4] = "ddde";
    Matrix M = create_peer_matrix(5, (char **)sequences);
    Particle p = {
        .value = 'd',
        .pos = {
            .row = 2,
            .num_cols = 2,
            .col = new int[2] {3, 4}
        }
    };
    CHECK(criteria3(p, 3, M));
}
