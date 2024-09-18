#pragma once

#include <cmath>
#include <concepts>
#include <numbers>
#include <ranges>

#include "psnr/common/defines.h"

namespace psnr::_v1 {

namespace rgs = std::ranges;

template <typename T>
[[nodiscard]] static constexpr T pow2(T num)
{
    return num * num;
}

template <typename T>
    requires std::is_unsigned_v<T>
[[nodiscard]] static inline double compute_mse(const T* plhs, const T* prhs, const size_t len)
{
    const T* lhs_cursor = plhs;
    const T* rhs_cursor = prhs;
    uint64_t acc = 0;
    for (size_t i = 0; i < len; i++) {
        int64_t l = *lhs_cursor;
        int64_t r = *rhs_cursor;
        int64_t diff = l - r;
        acc += pow2(diff);
        lhs_cursor++;
        rhs_cursor++;
    }

    const double mse = (double)acc / (double)len;
    return mse;
}

template <typename T, size_t depth = sizeof(T) * 8>
    requires std::is_unsigned_v<T> && (depth > 0)
[[nodiscard]] static inline double compute_psnr(const T* plhs, const T* prhs, const size_t len)
{
    const double mse = compute_mse(plhs, prhs, len);
    constexpr uint64_t maxval = 1 << depth;
    const double psnr = 10 * std::log10((double)pow2(maxval) / mse);
    return psnr;
}

} // namespace psnr::_v1

namespace psnr::v1 {

namespace _ = psnr::_v1;

using _::compute_psnr;

} // namespace psnr::v1
