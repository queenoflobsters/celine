#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

import celine;

TEST_CASE("Rank Test 1") {
    constexpr LinearApp<3, 3> app1 {
        { { 0, 0, 0 },   //
          { 0, 0, 0 },   //
          { 0, 0, 0 } }  //
    };
    CHECK(app1.rank() == 0);
}

TEST_CASE("Rank Test 2") {
    constexpr LinearApp<4, 4> app2 {
        { { 1, 0, 0, 0 },   //
          { 0, 1, 0, 0 },   //
          { 0, 0, 1, 0 },   //
          { 0, 0, 0, 1 } }  //
    };
    CHECK(app2.rank() == 4);
}

TEST_CASE("Rank Test 3") {
    constexpr LinearApp<3, 3> app3 {
        { { 1, 2, 3 },   //
          { 4, 5, 6 },   //
          { 7, 8, 9 } }  //
    };
    CHECK(app3.rank() == 2);
}

TEST_CASE("Rank Test 4") {
    constexpr LinearApp<3, 2> app4 {
        { { 1.5, 2.0, -1.0 },    //
          { -3.0, -4.0, 2.0 } }  //
    };
    CHECK(app4.rank() == 1);
}

TEST_CASE("Rank Test 5") {
    constexpr LinearApp<2, 4> app5 {
        { { 1, 0 },    //
          { 0, 1 },    //
          { 2, 3 },    //
          { -1, 4 } }  //
    };
    CHECK(app5.rank() == 2);
}

TEST_CASE("Rank Test 6") {
    constexpr LinearApp<4, 4> app6 {
        { { 1, 2, -1, 3 },   //
          { 0, 1, 4, 2 },    //
          { 1, 3, 3, 5 },    //
          { 2, 4, -2, 6 } }  //
    };
    CHECK(app6.rank() == 2);
}

TEST_CASE("Rank Test 7") {
    constexpr LinearApp<3, 3> app7({ { 0.0, 0.0, 0.0 },
                                     { 1.0, 2.0, 0.0 },
                                     { 0.0, 1.0, 3.0 } });

    CHECK(app7.rank() == 2);
}

TEST_CASE("Rank Test 8") {
    constexpr LinearApp<3, 3> app8({
        { 1.0, 2.0, 3.0 },
        { 4.0, 5.0, 6.0 },
        { 5.0, 7.0, 9.0 }  // R0 + R1
    });

    CHECK(app8.rank() == 2);
}

TEST_CASE("Rank Test 9") {
    constexpr LinearApp<4, 4> app9({ { 0.0, 1.0, 0.0, 0.0 },
                                     { 0.0, 0.0, 0.0, 1.0 },
                                     { 1.0, 0.0, 0.0, 0.0 },
                                     { 0.0, 0.0, 1.0, 0.0 } });

    CHECK(app9.rank() == 4);
}

TEST_CASE("Rank Test 10") {
    constexpr LinearApp<2, 4> app10({
        { 1.0, 0.0 },
        { 0.0, 1.0 },
        { 2.0, 3.0 },  // 2*R0 + 3*R1
        { 1.0, 1.0 }   // R0 + R1
    });

    CHECK(app10.rank() == 2);
}

TEST_CASE("Rank Test 11") {
    constexpr LinearApp app11({ { 2.0, -1.0 },
                                      { -4.0, 2.0 },
                                      { 6.0, -3.0 } });

    CHECK(app11.rank() == 1);
}

TEST_CASE("Rank Test 12") {
    constexpr LinearApp app12({ { 1.0, 2.0, 0.0, -1.0 },
                                      { 0.0, 1.0, -3.0, 2.0 } });

    CHECK(app12.rank() == 2);
}

TEST_CASE("Rank Test 13") {
    constexpr LinearApp app13({
        { 1.0, -2.0, 3.0, 4.0 },
        { -2.0, 4.0, -6.0, -8.0 }  // -2 * R0
    });

    CHECK(app13.rank() == 1);
}

TEST_CASE("Rank Test 14") {
    constexpr LinearApp app14({
        {0.0, 0.0, -5.0, 2.0}
    });

    CHECK(app14.rank() == 1);
}

TEST_CASE("Rank Test 15") {
    constexpr LinearApp app15({
        {0.0, 0.0, 0.0, 0.0}
    });

    CHECK(app15.rank() == 0);
}
