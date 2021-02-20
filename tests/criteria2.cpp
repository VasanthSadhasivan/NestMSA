#include "../catch.hpp"

#include "../nest_msa.h"

TEST_CASE("criteria2() Test 0", "[criteria2]") {
    Particle p;
    p.updated = 3;
    CHECK(criteria2(p, 2));
}

TEST_CASE("criteria2() Test 1", "[criteria2]") {
    Particle p;
    p.updated = 3;
    CHECK_FALSE(criteria2(p, 3));
}

TEST_CASE("criteria2() Test 2", "[criteria2]") {
    Particle p;
    p.updated = 3;
    CHECK_FALSE(criteria2(p, 4));
}
