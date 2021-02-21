#include "../catch.hpp"

#include "../nest_msa.h"

TEST_CASE("stopcriteria() Test 0", "[stopcriteria]") {
    const char *sequences[5];
    sequences[0] = "abbc";
    sequences[1] = "abcd";
    sequences[2] = "bbcd";
    sequences[3] = "cddd";
    sequences[4] = "ddde";
    Matrix M = create_peer_matrix(5, (char **)sequences);
    Particle p = {
        .value = 'b',
        .updated = 5,
        .pos = {
            .row = 0,
            .num_cols = 3,
            .col = new int[3] {0, 1, 2}
        }
    };
    CHECK_FALSE(stopcriteria(p, 1, M, 4));
}

TEST_CASE("stopcriteria() Test 1", "[stopcriteria]") {
    const char *sequences[5];
    sequences[0] = "abbc";
    sequences[1] = "abcd";
    sequences[2] = "bbcd";
    sequences[3] = "cddd";
    sequences[4] = "ddde";
    Matrix M = create_peer_matrix(5, (char **)sequences);
    Particle p = {
        .value = 'b',
        .updated = 4,
        .pos = {
            .row = 0,
            .num_cols = 1,
            .col = new int[1] {2}
        }
    };
    CHECK_FALSE(stopcriteria(p, 1, M));
}
