#pragma once

#include <concepts>
#include <cstdint>
#include <immintrin.h>
#include <limits>

#include "psnr/helper/constexpr/math.hpp"
#include "v1.hpp"

namespace psnr::_mse::v2 {

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
    const size_t simd_len = len / sizeof(__m128i);
    constexpr size_t step = sizeof(__m128i) / sizeof(uint8_t);
    constexpr size_t u8max = std::numeric_limits<uint8_t>::max();
    constexpr size_t u32max = std::numeric_limits<uint32_t>::max();
    constexpr size_t group_len = u32max / (u8max * u8max * 2);

    uint64_t sqrdiff_acc = 0;
    const __m256i zeromask = _mm256_setzero_si256();
    __m256i sqrdiff_simd_acc = zeromask;

    auto dump_unit = [&](const __m256i& u8l, const __m256i& u8r) mutable {
        const __m256i i16diff = _mm256_sub_epi16(u8l, u8r);
        const __m256i u16sqr = _mm256_mullo_epi16(i16diff, i16diff);
        const __m256i u32losqr = _mm256_unpacklo_epi16(u16sqr, zeromask);
        sqrdiff_simd_acc = _mm256_add_epi32(sqrdiff_simd_acc, u32losqr);
        const __m256i u32hisqr = _mm256_unpackhi_epi16(u16sqr, zeromask);
        sqrdiff_simd_acc = _mm256_add_epi32(sqrdiff_simd_acc, u32hisqr);
    };

    size_t count = 0;
    for (size_t i = 0; i < simd_len; i++) {
        const __m256i u8l = _mm256_cvtepu8_epi16(_mm_load_si128((__m128i*)lhs_cursor));
        const __m256i u8r = _mm256_cvtepu8_epi16(_mm_load_si128((__m128i*)rhs_cursor));
        dump_unit(u8l, u8r);
        lhs_cursor += step;
        rhs_cursor += step;

        count++;
        if (count == group_len) [[unlikely]] {
            auto* tmp = (uint32_t*)&sqrdiff_simd_acc;
            sqrdiff_acc += (tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7]);
            sqrdiff_simd_acc = zeromask;
            count = 0;
        }
    }

    auto* tmp = (uint32_t*)&sqrdiff_simd_acc;
    sqrdiff_acc += (tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7]);

    return sqrdiff_acc;
}

template <std::unsigned_integral Tv>
uint64_t MseOp_<Tv>::sqrdiff(const Tv* lhs, const Tv* rhs, size_t len) noexcept
{
    return v1::MseOp_<Tv>::sqrdiff(lhs, rhs, len);
}

using MseOpu8 = MseOp_<uint8_t>;

} // namespace psnr::_mse::v2

namespace psnr::mse::v2 {

namespace _ = _mse::v2;

using _::MseOp_;
using _::MseOpu8;

} // namespace psnr::mse::v2
