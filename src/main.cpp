#include <cassert>
#include <format>
#include <print>
#include <string_view>

import celine;

// Helpers
void print_section(std::string_view title) {
    std::println("\n{:═^70}", std::format("  {}  ", title));
}
template <std::size_t Dim>
void print_vector(std::string_view name, Vector<Dim> const& vec) {
    std::print("{} = (", name);
    for (std::size_t i = 0; i < Dim; ++i) {
        std::print("{}{}", vec[i], (i + 1 == Dim ? "" : ", "));
    }
    std::println(")");
}

int main() {
    // =========================================================================
    // 1. CONSTRUCTION MODES & FORMATTING
    // =========================================================================
    print_section("1. Construction Modes & Unicode Formatting");

    // From raw C-style nested arrays (R^3 -> R^3)
    constexpr LinearApp<3, 3> A({
        { 1.0,  2.0, 3.0 },
        { 0.0, -1.0, 4.0 },
        { 2.0,  1.0, 0.0 }
    });
    std::println("Constructed from 2D array (3x3):\n{}", A);

    // Rectangular application (Wide: R^4 -> R^2)
    constexpr LinearApp<4, 2> wide({
        { 1.0, 0.0, -2.0, 3.0 },
        { 0.0, 1.0,  4.0, 0.0 }
    });
    std::println("Rectangular Wide Matrix (R^4 -> R^2):\n{}", wide);

    // Rectangular application (Tall: R^2 -> R^3)
    constexpr LinearApp<2, 3> tall({
        { 1.0, 0.0 },
        { 0.0, 1.0 },
        { 1.0, 1.0 }
    });
    std::println("Rectangular Tall Matrix (R^2 -> R^3):\n{}", tall);

    // From predefined Matrix alias
    constexpr Matrix<2, 2> raw_mat = {{ { 5.0, -2.0 }, { 3.0, 1.0 } }};
    constexpr LinearApp B { raw_mat };
    std::println("Constructed from Matrix<2, 2> alias:\n{}", B);

    // =========================================================================
    // 2. LINEAR FORM DSL (Natural Expressions with X, Y, Z, T)
    // =========================================================================
    print_section("2. Linear Forms & Coordinate DSL (X, Y, Z, T)");

    // Define linear forms as natural mathematical combinations
    constexpr LinearForm form1 = 3.0 * X - 2.0 * Y + 4.5 * Z;
    constexpr LinearForm form2 = -X + 5.0 * Y - Z + 2.0 * T;

    std::println("Linear form 1 (in R^3): {}", form1);
    std::println("Linear form 2 (in R^4): {}", form2);

    // Assemble an entire LinearApp line-by-line using the fold-expression constructor!
    LinearApp<3, 3> system {
        X + 2.0 * Y + 3.0 * Z,
        2.0 * X - Y + Z,
        3.0 * X + Y - 2.0 * Z
    };
    std::println("Matrix assembled row-by-row via Linear Forms:\n{}", system);

    // =========================================================================
    // 3. APPLICATION & EVALUATION (Mapping Vectors f(v))
    // =========================================================================
    print_section("3. Linear Mapping Application: f(v)");

    // Application with a Vector object
    constexpr Vector<3> v_in { 1.0, 2.0, 3.0 };
    Vector<3> v_out = system(v_in);

    print_vector("Input vector v", v_in);
    print_vector("Result system(v)", v_out);

    // Application using parameter pack syntax
    Vector<3> direct_out = system(1.0, 0.0, -1.0);
    print_vector("Direct evaluation system(1, 0, -1)", direct_out);

    // Rectangular mapping application: R^4 -> R^2
    Vector<2> wide_out = wide(1.0, 2.0, 3.0, 4.0);
    print_vector("wide(1, 2, 3, 4) in R^2", wide_out);

    // =========================================================================
    // 4. ARITHMETIC OPERATORS
    // =========================================================================
    print_section("4. Matrix Arithmetic Operations");

    constexpr LinearApp<3, 3> M1({
        { 1.0, 2.0, 0.0 },
        { 0.0, 1.0, 1.0 },
        { 1.0, 0.0, 2.0 }
    });

    constexpr LinearApp<3, 3> M2({
        { 2.0, -1.0, 1.0 },
        { 1.0,  0.0, 2.0 },
        { 0.0,  3.0, 1.0 }
    });

    std::println("M1:\n{}", M1);
    std::println("M2:\n{}", M2);
    std::println("Addition (M1 + M2):\n{}", M1 + M2);
    std::println("Subtraction (M1 - M2):\n{}", M1 - M2);
    std::println("Negation (-M1):\n{}", -M1);
    std::println("Scalar Multiplication (2.5 * M1):\n{}", 2.5 * M1);
    std::println("Scalar Division (M2 / 2.0):\n{}", M2 / 2.0);

    // Compound assignments
    LinearApp<3, 3> M_accum = M1;
    M_accum += M2;
    M_accum *= 2.0;
    std::println("Compound result ((M1 + M2) * 2):\n{}", M_accum);

    // =========================================================================
    // 5. RANK, INJECTIVITY, SURJECTIVITY, BIJECTIVITY
    // =========================================================================
    print_section("5. Rank & Morphism Properties");

    // Case A: Full-rank square matrix (Bijective / Isomorphism)
    constexpr LinearApp<3, 3> invertible({
        { 1.0, 0.0, 2.0 },
        { 0.0, 1.0, 1.0 },
        { 2.0, 1.0, 6.0 }
    });
    std::println("Invertible Matrix:\n{}", invertible);
    std::println(" -> Rank:        {}", invertible.rank());
    std::println(" -> Injective:   {}", invertible.injective());
    std::println(" -> Surjective:  {}", invertible.surjective());
    std::println(" -> Bijective:   {}", invertible.bijective());

    // Case B: Rank-deficient matrix (R2 = R0 + R1)
    constexpr LinearApp<3, 3> singular({
        { 1.0, 2.0, 3.0 },
        { 4.0, 5.0, 6.0 },
        { 5.0, 7.0, 9.0 }
    });
    std::println("Singular Matrix (R2 = R0 + R1):\n{}", singular);
    std::println(" -> Rank:        {}", singular.rank());
    std::println(" -> Bijective:   {}", singular.bijective());

    // Case C: Wide matrix (Surjective test: R^4 -> R^2)
    std::println("Wide Matrix (R^4 -> R^2):\n{}", wide);
    std::println(" -> Rank:        {}", wide.rank());
    std::println(" -> Injective:   {}", wide.injective());
    std::println(" -> Surjective:  {}", wide.surjective());

    // Case D: Tall matrix (Injective test: R^2 -> R^3)
    std::println("Tall Matrix (R^2 -> R^3):\n{}", tall);
    std::println(" -> Rank:        {}", tall.rank());
    std::println(" -> Injective:   {}", tall.injective());
    std::println(" -> Surjective:  {}", tall.surjective());

    // =========================================================================
    // 6. ENDOMORPHISM & PROJECTOR (P^2 = P)
    // =========================================================================
    print_section("6. Projector Detection (P^2 == P)");

    // Projection onto the xy-plane in R^3
    constexpr LinearApp<3, 3> P_xy({
        { 1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0 },
        { 0.0, 0.0, 0.0 }
    });

    std::println("Projection onto xy-plane:\n{}", P_xy);
    std::println(" -> Is projector: {}", P_xy.projector());
    std::println(" -> M1 is projector: {}", M1.projector());

    // =========================================================================
    // 7. COMPILE-TIME EVALUATION GUARANTEE (constexpr / static_assert)
    // =========================================================================
    print_section("7. Static Assertions (All verified at Compile-Time)");

    static_assert(invertible.rank() == 3);
    static_assert(invertible.bijective());
    static_assert(singular.rank() == 2);
    static_assert(!singular.bijective());
    static_assert(wide.rank() == 2);
    static_assert(wide.surjective() && !wide.injective());
    static_assert(tall.rank() == 2);
    static_assert(tall.injective() && !tall.surjective());
    static_assert(P_xy.projector());

    // Compile-time matrix operations
    constexpr auto scaled_inv = invertible * 2.0;
    static_assert(scaled_inv.rank() == 3);

    std::println("✓ All static assertions passed successfully at compile-time!");
    return 0;
}
