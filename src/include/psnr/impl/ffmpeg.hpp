#pragma once

#include <cmath>
#include <numbers>
#include <ranges>

#include "psnr/common/defines.h"

namespace psnr::_ffmpeg {

namespace rgs = std::ranges;

template <typename T>
static constexpr T pow2(T num)
{
    return num * num;
}

static inline double compute_psnr(const uint8_t* plhs, const uint8_t* prhs, const size_t len)
{
    const uint8_t* lhs_cursor = plhs;
    const uint8_t* rhs_cursor = prhs;
    uint64_t acc = 0;
    for (size_t i = 0; i < len; i++) {
        int64_t l = *lhs_cursor;
        int64_t r = *rhs_cursor;
        int64_t diff = l - r;
        acc += pow2(diff);
    }

    const double mse = (double)acc / (double)len;
    const double psnr = 10 * std::log10(pow2(std::numeric_limits<uint8_t>::max()) / mse);
    return psnr;
}

} // namespace psnr::_ffmpeg

namespace psnr::ffmpeg {

namespace _priv = psnr::_ffmpeg;

using _priv::compute_psnr;

} // namespace psnr::ffmpeg
