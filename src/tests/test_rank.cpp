#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

import celine;

TEST_CASE("Rank Test 1") {
    LinearApp<3, 3> app1 {
        { { 0, 0, 0 },   //
          { 0, 0, 0 },   //
          { 0, 0, 0 } }  //
    };
    CHECK(app1.rank() == 0);
}

TEST_CASE("Rank Test 2") {
    LinearApp<4, 4> app2 {
        { { 1, 0, 0, 0 },   //
          { 0, 1, 0, 0 },   //
          { 0, 0, 1, 0 },   //
          { 0, 0, 0, 1 } }  //
    };
    CHECK(app2.rank() == 4);
}

TEST_CASE("Rank Test 3") {
    LinearApp<3, 3> app3 {
        { { 1, 2, 3 },   //
          { 4, 5, 6 },   //
          { 7, 8, 9 } }  //
    };
    CHECK(app3.rank() == 2);
}

TEST_CASE("Rank Test 4") {
    LinearApp<3, 2> app4 {
        { { 1.5, 2.0, -1.0 },    //
          { -3.0, -4.0, 2.0 } }  //
    };
    CHECK(app4.rank() == 1);
}

TEST_CASE("Rank Test 5") {
    LinearApp<2, 4> app5 {
        { { 1, 0 },    //
          { 0, 1 },    //
          { 2, 3 },    //
          { -1, 4 } }  //
    };
    CHECK(app5.rank() == 2);
}

TEST_CASE("Rank Test 6") {
    LinearApp<4, 4> app6 {
        { { 1, 2, -1, 3 },   //
          { 0, 1, 4, 2 },    //
          { 1, 3, 3, 5 },    //
          { 2, 4, -2, 6 } }  //
    };
    CHECK(app6.rank() == 2);
}
