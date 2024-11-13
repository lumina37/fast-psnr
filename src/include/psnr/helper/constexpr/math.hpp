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
[[nodiscard]] static constexpr inline T roundTo(T v)
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
[[nodiscard]] static constexpr inline bool isPowOf2(const Tv v)
{
    return (v & (v - 1)) == 0;
}

template <size_t base, std::integral T>
    requires(isPowOf2(base))
[[nodiscard]] static constexpr inline bool isMulOf(T v)
{
    return (v & (1 << log2(base))) == 0;
};

template <size_t to, std::integral T>
    requires(isPowOf2(to))
[[nodiscard]] static constexpr inline T alignUp(T v)
{
    return (v + (to - 1)) & ((~to) + 1);
};

template <size_t to, typename T>
    requires std::is_pointer_v<T> && (isPowOf2(to))
[[nodiscard]] static constexpr inline T alignUp(T v)
{
    return (T)alignUp<to>((size_t)v);
};

template <size_t to, std::integral T>
    requires(isPowOf2(to))
[[nodiscard]] static constexpr inline T alignDown(T v)
{
    return v & ((~to) + 1);
};

template <size_t to, typename T>
    requires std::is_pointer_v<T> && (isPowOf2(to))
[[nodiscard]] static constexpr inline T alignDown(T v)
{
    return (T)alignDown<to>((size_t)v);
};

} // namespace psnr::_hp
