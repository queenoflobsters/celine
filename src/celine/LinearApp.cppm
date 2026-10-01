module;

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <print>
#include <utility>

export module celine:LinearApp;

// floating point approx
constexpr double EPSILON = 1e-9;

// mat[row][col] -> OutDim rows, InDim columns
// f: R^4 -> R^3
// 0 0 0 0
// 0 0 0 0
// 0 0 0 0
export template <std::size_t Row, std::size_t Col>
using Matrix = std::array<std::array<double, Col>, Row>;

export template <std::size_t Dim>
using Vector = std::array<double, Dim>;

export template <std::size_t InDim, std::size_t OutDim>
class LinearApp {
public:
    // c'est définitivement comme ça la meilleure manière de stocker mat !
    Matrix<OutDim, InDim> mat;

    constexpr LinearApp() noexcept : mat {} {};

    /// CONSTRUCTORS ///

    // ugly but here for inline construction
    constexpr LinearApp(double const (&raw)[OutDim][InDim]) noexcept {  // NOLINT
        for (size_t out = 0; out < OutDim; ++out) {
            for (size_t in = 0; in < InDim; ++in) {
                mat[out][in] = raw[out][in];
            }
        }
        // unfortunatly doesn't work in constexpr
        // std::copy(raw.begin(), raw.end(), mat.begin());
    }

    // initialization directly from another matrix
    constexpr LinearApp(Matrix<OutDim, InDim> const& other_mat) : mat { other_mat } {}

    // creates a placeholder by only setting
    // the last value of the linear form
    static constexpr LinearApp placeholder() noexcept
        requires(InDim >= 1 && OutDim == 1)
    {
        LinearApp app {};
        app.mat[0].back() = 1.0;
        return app;
    }

    // magic constructor for natural expressions
    template <std::size_t... LinesInDims>
        requires((LinesInDims <= InDim) && ...) && (sizeof...(LinesInDims) == OutDim)
    constexpr LinearApp(LinearApp<LinesInDims, 1> const&... lines) noexcept : mat {} {  // NOLINT
        // I know state mutating fold expressions in lambdas is an anti-pattern
        // I just don't care because the alternatives are damn ugly
        int out = 0;
        ([&, this] {
            for (size_t in = 0; in < LinesInDims; ++in) {
                mat[out][in] = lines.mat[0][in];
            }
            out++;
        }(),
         ...);
    }

    /// MEMBER FUNCTIONS ///

    [[nodiscard]] constexpr size_t in_dim() const noexcept {
        return InDim;
    }

    [[nodiscard]] constexpr size_t out_dim() const noexcept {
        return OutDim;
    }

    [[nodiscard]] constexpr bool projector() const noexcept
        requires(InDim == OutDim)
    {
        LinearApp sq {};
        // ikj loop to satify cache locality because mat is row-major
        for (size_t i = 0; i < OutDim; ++i) {
            for (size_t k = 0; k < OutDim; ++k) {
                double scalar = mat[i][k];
                for (size_t j = 0; j < OutDim; ++j) {
                    sq.mat[i][j] += scalar * mat[k][j];
                }
            }
        }
        return sq == mat;
    }

    [[nodiscard]] constexpr size_t rank() const noexcept {
        Matrix temp { mat };

        for (size_t i1 = 0; i1 < OutDim; ++i1) {
            std::println("{}", temp);
            // Find the first non zero of the line
            size_t first_non_zero = 0;
            bool row_empty = true;
            for (size_t j = 0; j < InDim; ++j) {
                if (std::abs(temp[i1][j]) > EPSILON) {  // TODO epsilon
                    row_empty = false;
                    first_non_zero = j;
                    break;
                }
            }
            if (row_empty) {
                std::println("Row empty return: {}", i1);
                return i1;
            }

            for (size_t i2 = i1 + 1; i2 < OutDim; ++i2) {
                for (size_t j = 0; j < InDim; ++j) {
                    if (j == first_non_zero) {
                        temp[i2][j] = 0.0;
                    } else {
                        std::println("first_non_zero : {}", first_non_zero);
                        std::println("temp[i1][first_non_zero] : {}", temp[i1][first_non_zero]);
                        std::println("i2:{}, j:{}, ... : {}", i2, j, temp[i2][j] * (temp[i2][first_non_zero] / temp[i1][first_non_zero]));
                        temp[i2][j] -= temp[i2][j] * (temp[i2][first_non_zero] / temp[i1][first_non_zero]);
                    }
                }
            }
        }

        std::println("Final branch : {}", OutDim);

        return OutDim;
    }

    [[nodiscard]] constexpr bool surjective() const noexcept;
    [[nodiscard]] constexpr bool injective() const noexcept;
    [[nodiscard]] constexpr bool bijective() const noexcept;

    /// OPERATORS ///

    constexpr Vector<OutDim> operator()(Vector<InDim> const& in_vec) const noexcept {
        Vector<OutDim> out_vec;
        for (size_t out = 0; out < OutDim; ++out) {
            double term = 0.;
            for (size_t in = 0; in < InDim; ++in) {
                term += mat[out][in] * in_vec[in];
            }
            out_vec[out] = term;
        }

        return out_vec;
    }

    template <typename... Args>
        requires(sizeof...(Args) == InDim) && (std::convertible_to<double, Args> && ...)
    constexpr Vector<OutDim> operator()(Args... in_vec) const noexcept {
        return this->operator()(Vector<InDim> { static_cast<double>(in_vec)... });
    }

    constexpr LinearApp& operator+=(LinearApp const& rhs) noexcept {
        for (size_t out = 0; out < OutDim; ++out) {
            for (size_t in = 0; in < InDim; ++in) {
                mat[out][in] += rhs.mat[out][in];
            }
        }
        return *this;
    }

    constexpr LinearApp& operator-=(LinearApp const& rhs) noexcept {
        for (size_t out = 0; out < OutDim; ++out) {
            for (size_t in = 0; in < InDim; ++in) {
                mat[out][in] -= rhs.mat[out][in];
            }
        }
        return *this;
    }

    constexpr LinearApp& operator*=(double const coef) noexcept {
        for (auto& row : mat) {
            for (auto& val : row) {
                val *= coef;
            }
        }
        return *this;
    }

    // passed lhs by value to enable move semantics and optimizations
    friend constexpr LinearApp operator+(LinearApp lhs, LinearApp const& rhs) noexcept {
        lhs += rhs;
        return lhs;
    }

    friend constexpr LinearApp operator-(LinearApp app) noexcept {
        app *= -1.0;
        return app;
    }

    friend constexpr LinearApp operator-(LinearApp lhs, LinearApp const& rhs) noexcept {
        lhs -= rhs;
        return lhs;
    }

    friend constexpr LinearApp operator*(double coef, LinearApp app) noexcept {
        app *= coef;
        return app;
    }

    friend constexpr LinearApp operator*(LinearApp app, double coef) noexcept {
        app *= coef;
        return app;
    }

    friend constexpr LinearApp operator/(LinearApp app, double coef) noexcept {
        app *= (1. / coef);
        return app;
    }
};

export template <std::size_t FirstInDim, std::size_t SecondInDim, size_t RetInDim = std::max(FirstInDim, SecondInDim)>
constexpr LinearApp<RetInDim, 1> operator+(LinearApp<FirstInDim, 1> const& lhs, LinearApp<SecondInDim, 1> const& rhs) noexcept {
    return [&]<std::size_t... Ins>(std::index_sequence<Ins...>) {
        return LinearApp<RetInDim, 1> { { { (
            (Ins < FirstInDim ? lhs.mat[0][Ins] : 0.0) +
            (Ins < SecondInDim ? rhs.mat[0][Ins] : 0.0))... } } };
    }(std::make_index_sequence<RetInDim>());
}

// not very DRY but I don't have a better idea
export template <std::size_t FirstInDim, std::size_t SecondInDim, size_t RetInDim = std::max(FirstInDim, SecondInDim)>
constexpr LinearApp<RetInDim, 1> operator-(LinearApp<FirstInDim, 1> const& lhs, LinearApp<SecondInDim, 1> const& rhs) noexcept {
    return [&]<std::size_t... Ins>(std::index_sequence<Ins...>) {
        return LinearApp<RetInDim, 1> { { { (
            (Ins < FirstInDim ? lhs.mat[0][Ins] : 0.0) -
            (Ins < SecondInDim ? rhs.mat[0][Ins] : 0.0))... } } };
    }(std::make_index_sequence<RetInDim>());
}

export template <size_t Dim>
using LinearForm = LinearApp<Dim, 1>;

export inline constexpr LinearApp X = LinearForm<1>::placeholder();
export inline constexpr LinearApp Y = LinearForm<2>::placeholder();
export inline constexpr LinearApp Z = LinearForm<3>::placeholder();
export inline constexpr LinearApp T = LinearForm<4>::placeholder();

// Formatting stuff
template <std::size_t InDim, size_t OutDim>
struct std::formatter<LinearApp<InDim, OutDim>> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    constexpr auto format(LinearApp<InDim, OutDim> const& app, std::format_context& ctx) const {
        std::string result = "\n";
        for (size_t i = 0; i < OutDim; ++i) {
            if (OutDim == 1) {
                result += "[  ";
            } else if (i == 0) {
                result += "┏  ";
            } else if (i + 1 == OutDim) {
                result += "┗  ";
            } else {
                result += "┃  ";
            }
            for (size_t j = 0; j < InDim; ++j) {
                result += std::format("{}", app.mat[i][j]) + (j == InDim - 1 ? "" : "\t");
            }

            if (OutDim == 1) {
                result += "\t]\n";
            } else if (i == 0) {
                result += "\t┓\n";
            } else if (i + 1 == OutDim) {
                result += "\t┛\n";
            } else {
                result += "\t┃\n";
            }
        }
        return std::format_to(ctx.out(), "{}", result);
    }
};
