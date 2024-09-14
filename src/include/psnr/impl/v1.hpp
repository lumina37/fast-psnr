#pragma once

#include <cmath>
#include <concepts>
#include <numbers>
#include <ranges>

#include "psnr/common/defines.h"

namespace psnr::_v1 {

namespace rgs = std::ranges;

template <typename TR, typename TP>
[[nodiscard]] static constexpr TR pow2(TP num)
{
    return (TR)num * (TR)num;
}

template <typename T>
    requires std::is_unsigned_v<T>
[[nodiscard]] static inline double compute(const T* plhs, const T* prhs, const size_t len)
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

} // namespace psnr::_v1

namespace psnr::v1 {

namespace _ = psnr::_v1;

using _::compute;

} // namespace psnr::v1
