#include "../catch.hpp"

#include <string.h>
#include "../nest_msa.h"

TEST_CASE("create_swarm() Test 0", "[create_swarm]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    Particle a = {
        .value = 'a',
        .pos = {
            .row = 0,
            .num_cols = 4,
            .col = new int[4] {0, 1, 2, 3}
        }
    };
    Swarm correct = {
        .num_particles = 1,
        .swarm = new Particle[1] {a}
    };
    Swarm s = create_swarm(0, M);
    CHECK(s.num_particles == correct.num_particles);
    for (int i = 0; i < correct.num_particles; i++) {
        CHECK(s.swarm[i].value == correct.swarm[i].value);
        CHECK(s.swarm[i].pos.row == correct.swarm[i].pos.row);
        CHECK(s.swarm[i].pos.num_cols == correct.swarm[i].pos.num_cols);
        CHECK(memcmp(s.swarm[i].pos.col, correct.swarm[i].pos.col, correct.swarm[i].pos.num_cols) == 0);
    }
}

TEST_CASE("create_swarm() Test 1", "[create_swarm]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    Particle b = {
        .value = 'b',
        .pos = {
            .row = 1,
            .num_cols = 3,
            .col = new int[3] {0, 2, 3}
        }
    };
    Particle c = {
        .value = 'c',
        .pos = {
            .row = 1,
            .num_cols = 1,
            .col = new int[1] {1}
        }
    };
    Swarm correct = {
        .num_particles = 2,
        .swarm = new Particle[2] {b, c}
    };
    Swarm s = create_swarm(1, M);
    CHECK(s.num_particles == correct.num_particles);
    for (int i = 0; i < correct.num_particles; i++) {
        CHECK(s.swarm[i].value == correct.swarm[i].value);
        CHECK(s.swarm[i].pos.row == correct.swarm[i].pos.row);
        CHECK(s.swarm[i].pos.num_cols == correct.swarm[i].pos.num_cols);
        CHECK(memcmp(s.swarm[i].pos.col, correct.swarm[i].pos.col, correct.swarm[i].pos.num_cols) == 0);
    }
}

TEST_CASE("create_swarm() Test 2", "[create_swarm]") {
    const char *sequences[4];
    sequences[0] = "abcbcdem";
    sequences[1] = "acbcfg";
    sequences[2] = "abchimn";
    sequences[3] = "abcbcjkm";
    Matrix M = create_peer_matrix(4, (char **)sequences);
    Particle m = {
        .value = 'm',
        .pos = {
            .row = 7,
            .num_cols = 2,
            .col = new int[2] {0, 3}
        }
    };
    Swarm correct = {
        .num_particles = 1,
        .swarm = new Particle[1] {m}
    };
    Swarm s = create_swarm(7, M);
    CHECK(s.num_particles == correct.num_particles);
    for (int i = 0; i < correct.num_particles; i++) {
        CHECK(s.swarm[i].value == correct.swarm[i].value);
        CHECK(s.swarm[i].pos.row == correct.swarm[i].pos.row);
        CHECK(s.swarm[i].pos.num_cols == correct.swarm[i].pos.num_cols);
        CHECK(memcmp(s.swarm[i].pos.col, correct.swarm[i].pos.col, correct.swarm[i].pos.num_cols) == 0);
    }
}
