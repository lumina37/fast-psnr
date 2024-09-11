#pragma once

#include <bit>
#include <concepts>

namespace psnr::_hp {

template <size_t to, typename T>
    requires std::is_integral_v<T> && (to % 2 == 0)
constexpr T round_to(T v)
{
    constexpr T half_to = to >> 1;
    return (v + half_to) / to * to;
}

template <typename Tv>
    requires std::is_unsigned_v<Tv>
constexpr Tv log2(const Tv v)
{
    return std::bit_width(v) - 1;
}

template <size_t base, typename T>
    requires std::is_integral_v<T> && (base % 2 == 0)
constexpr bool is_mul_of(T v)
{
    return (v & (1 << log2(base))) == 0;
};

template <typename Tv>
    requires std::is_unsigned_v<Tv>
constexpr bool is_pow_of_2(const Tv v)
{
    return (v & (v - 1)) == 0;
}

template <size_t to, typename T>
    requires std::is_integral_v<T> && (is_pow_of_2(to))
constexpr T align_to(T v)
{
    return (v + (to - 1)) & -to;
};

} // namespace psnr::_hp
