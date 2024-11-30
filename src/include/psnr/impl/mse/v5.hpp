﻿#pragma once

#include <array>
#include <concepts>
#include <cstdint>
#include <immintrin.h>
#include <limits>

#include "psnr/helper/constexpr/math.hpp"
#include "v1.hpp"

namespace psnr::_mse::v5 {

template <std::unsigned_integral Tv_>
class MseOp_
{
public:
    using Tv = Tv_;

    [[nodiscard]] static inline double mse(const Tv* lhs, const Tv* rhs, const size_t len) noexcept
    {
        const Tv* lhead = _hp::alignUp<sizeof(__m128i)>(lhs);
        const Tv* rhead = _hp::alignUp<sizeof(__m128i)>(rhs);

        const size_t lgap = lhead - lhs;
        if (lgap != (rhead - rhs)) [[unlikely]] {
            // not the same alignment
            // simd load will suffer
            return v1::MseOp_<Tv>::mse(lhs, rhs, len);
        }

        const Tv* ltail = _hp::alignDown<sizeof(__m128i)>(lhs + len);
        const Tv* rtail = _hp::alignDown<sizeof(__m128i)>(rhs + len);

        if (lhead == ltail) [[unlikely]] {
            // len is too small
            // unable to assemble an entire __m128i
            return v1::MseOp_<Tv>::mse(lhs, rhs, len);
        }

        uint64_t sqrdiff_acc = 0;
        sqrdiff_acc += v1::MseOp_<Tv>::sqrdiff(lhead, rhead, lhead - lhs);
        sqrdiff_acc += sqrdiff(lhead, rhead, ltail - lhead);
        sqrdiff_acc += v1::MseOp_<Tv>::sqrdiff(ltail, rtail, lhs + len - ltail);

        const double mse = (double)sqrdiff_acc / (double)len;
        return mse;
    }

    [[nodiscard]] static inline uint64_t sqrdiff(const Tv* lhs, const Tv* rhs, size_t len) noexcept;
};

template <>
uint64_t MseOp_<uint8_t>::sqrdiff(const uint8_t* lhs, const uint8_t* rhs, size_t len) noexcept
{
    const uint8_t* lhs_cursor = lhs;
    const uint8_t* rhs_cursor = rhs;
    const size_t m128_cnt = len / sizeof(__m128i);
    constexpr size_t step = sizeof(__m128i) / sizeof(uint8_t);
    constexpr size_t u8max = std::numeric_limits<uint8_t>::max();
    constexpr size_t u32max = std::numeric_limits<uint32_t>::max();
    // 做4x循环展开
    constexpr size_t unroll = 8;
    constexpr size_t group_len = (u32max / (u8max * u8max * 2)) * unroll;
    const size_t group_cnt = m128_cnt / group_len;
    // 还有`nogroup_len`个`__m128i`不成组
    const size_t nogroup_len = m128_cnt - group_cnt * group_len;
    // 将它们打包成`nogroup_unroll_cnt`个长`unroll`的小组以执行循环展开
    const size_t nogroup_unroll_cnt = nogroup_len / unroll;
    // 剩下`resi_cnt`个`__m128i`无法做向量展开，单独处理
    const size_t resi_cnt = nogroup_len - nogroup_unroll_cnt * unroll;

    uint64_t sqr_diff_acc = 0;
    // 使用独立的累加器
    std::array<__m256i, unroll> u32sqr_diff_acc{};

    auto dump_unit = [&](const __m256i u8l, const __m256i u8r, const size_t i) mutable {
        const __m256i i16diff = _mm256_sub_epi16(u8l, u8r);
        const __m256i u32sqr_diff = _mm256_madd_epi16(i16diff, i16diff);
        u32sqr_diff_acc[i] = _mm256_add_epi32(u32sqr_diff_acc[i], u32sqr_diff);
    };

    auto dump_u32sqr_diff_acc = [&]() mutable {
        for (size_t i = 0; i < unroll; i++) {
            __m256i u64sqr_diff_acc_p0 = _mm256_cvtepu32_epi64(_mm256_extractf128_si256(u32sqr_diff_acc[i], 0));
            __m256i u64sqr_diff_acc_p1 = _mm256_cvtepu32_epi64(_mm256_extractf128_si256(u32sqr_diff_acc[i], 1));
            __m256i u64sqr_diff_acc = _mm256_add_epi64(u64sqr_diff_acc_p0, u64sqr_diff_acc_p1);
            auto* tmp = (uint64_t*)&u64sqr_diff_acc;
            sqr_diff_acc += (tmp[0] + tmp[1] + tmp[2] + tmp[3]);
        }
    };

    for (size_t igroup = 0; igroup < group_cnt; igroup++) {
        for (size_t i = 0; i < (group_len / unroll); i++) {
            for (size_t j = 0; j < unroll; j++) {
                const __m256i u8l = _mm256_cvtepu8_epi16(_mm_load_si128((__m128i*)lhs_cursor));
                const __m256i u8r = _mm256_cvtepu8_epi16(_mm_load_si128((__m128i*)rhs_cursor));
                dump_unit(u8l, u8r, j);
                lhs_cursor += step;
                rhs_cursor += step;
            }
        }

        dump_u32sqr_diff_acc();
        for (size_t j = 0; j < unroll; j++) {
            u32sqr_diff_acc[j] = _mm256_setzero_si256();
        }
    }

    for (size_t i = 0; i < nogroup_unroll_cnt; i++) {
        for (size_t j = 0; j < unroll; j++) {
            const __m256i u8l = _mm256_cvtepu8_epi16(_mm_load_si128((__m128i*)lhs_cursor));
            const __m256i u8r = _mm256_cvtepu8_epi16(_mm_load_si128((__m128i*)rhs_cursor));
            dump_unit(u8l, u8r, j);
            lhs_cursor += step;
            rhs_cursor += step;
        }
    }

    for (size_t i = 0; i < resi_cnt; i++) {
        const __m256i u8l = _mm256_cvtepu8_epi16(_mm_load_si128((__m128i*)lhs_cursor));
        const __m256i u8r = _mm256_cvtepu8_epi16(_mm_load_si128((__m128i*)rhs_cursor));
        dump_unit(u8l, u8r, i);
        lhs_cursor += step;
        rhs_cursor += step;
    }

    dump_u32sqr_diff_acc();

    return sqr_diff_acc;
}

template <std::unsigned_integral Tv>
uint64_t MseOp_<Tv>::sqrdiff(const Tv* lhs, const Tv* rhs, size_t len) noexcept
{
    return v1::MseOp_<Tv>::sqrdiff(lhs, rhs, len);
}

using MseOpu8 = MseOp_<uint8_t>;

} // namespace psnr::_mse::v5

namespace psnr::mse::v5 {

namespace _ = _mse::v5;

using _::MseOp_;
using _::MseOpu8;

} // namespace psnr::mse::v5
