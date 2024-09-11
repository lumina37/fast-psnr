#pragma once

#include <cmath>
#include <concepts>
#include <numbers>
#include <ranges>

#include "psnr/common/defines.h"

namespace psnr::_native {

namespace rgs = std::ranges;

template <typename TR, typename TI>
[[nodiscard]] static constexpr TR pow2(TI num)
{
    return (TR)num * (TR)num;
}

template <typename T>
    requires std::is_unsigned_v<T>
[[nodiscard]] static inline double compute_psnr(const T* plhs, const T* prhs, const size_t len)
{
    const T* lhs_cursor = plhs;
    const T* rhs_cursor = prhs;
    uint64_t acc = 0;
    for (size_t i = 0; i < len; i++) {
        int64_t l = *lhs_cursor;
        int64_t r = *rhs_cursor;
        int64_t diff = l - r;
        acc += pow2<decltype(acc)>(diff);
        lhs_cursor++;
        rhs_cursor++;
    }

    const double mse = (double)acc / (double)len;
    const double psnr = 10 * std::log10(pow2<double>(std::numeric_limits<T>::max()) / mse);
    return psnr;
}

} // namespace psnr::_native

namespace psnr::native {

namespace _ = psnr::_native;

using _::compute_psnr;

} // namespace psnr::native
