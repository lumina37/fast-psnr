#pragma once

#include <bit>
#include <concepts>

namespace psnr::_hp {

template <typename T>
[[nodiscard]] static constexpr inline T pow2(T num)
{
    return num * num;
}

template <size_t to, std::integral T>
    requires(to % 2 == 0)
[[nodiscard]] static constexpr inline T round_to(T v)
{
    constexpr T half_to = to >> 1;
    return (v + half_to) / to * to;
}

template <std::unsigned_integral Tv>
[[nodiscard]] static constexpr inline Tv log2(const Tv v)
{
    return std::bit_width(v) - 1;
}

template <std::unsigned_integral Tv>
[[nodiscard]] static constexpr inline bool is_pow_of_2(const Tv v)
{
    return (v & (v - 1)) == 0;
}

template <size_t base, std::integral T>
    requires(is_pow_of_2(base))
[[nodiscard]] static constexpr inline bool is_mul_of(T v)
{
    return (v & (1 << log2(base))) == 0;
};

template <size_t to, std::integral T>
    requires(is_pow_of_2(to))
[[nodiscard]] static constexpr inline T align_up(T v)
{
    return (v + (to - 1)) & ((~to) + 1);
};

template <size_t to, typename T>
    requires std::is_pointer_v<T> && (is_pow_of_2(to))
[[nodiscard]] static constexpr inline T align_up(T v)
{
    return (T)align_up<to>((size_t)v);
};

template <size_t to, std::integral T>
    requires(is_pow_of_2(to))
[[nodiscard]] static constexpr inline T align_down(T v)
{
    return v & ((~to) + 1);
};

template <size_t to, typename T>
    requires std::is_pointer_v<T> && (is_pow_of_2(to))
[[nodiscard]] static constexpr inline T align_down(T v)
{
    return (T)align_down<to>((size_t)v);
};

} // namespace psnr::_hp
