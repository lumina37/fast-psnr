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
    // 以__m128i为加载单元，`len`中共包含`simd_len`组__m128i
    const size_t simd_len = len / sizeof(__m128i);
    // step是加载步长，也就是16字节
    constexpr size_t step = sizeof(__m128i) / sizeof(uint8_t);
    // 为避免uint32溢出，每隔`group_len`组就需要把累加值dump一次到uint64
    // 最后的`*2`是因为每一组uint16都对应了两组uint32
    constexpr size_t u8max = std::numeric_limits<uint8_t>::max();
    constexpr size_t u32max = std::numeric_limits<uint32_t>::max();
    constexpr size_t group_len = u32max / (u8max * u8max * 2);

    uint64_t sqdacc = 0;
    __m256i u32sqdacc = _mm256_setzero_si256();

    // 累加一组SIMD向量
    auto dump_unit = [&](const __m256i& u8l, const __m256i& u8r) mutable {
        const __m256i i16diff = _mm256_sub_epi16(u8l, u8r);
        const __m256i u16sqd = _mm256_mullo_epi16(i16diff, i16diff);
        const __m256i u32sqd_lo = _mm256_unpacklo_epi16(u16sqd, _mm256_setzero_si256());
        u32sqdacc = _mm256_add_epi32(u32sqdacc, u32sqd_lo);
        const __m256i u32sqd_hi = _mm256_unpackhi_epi16(u16sqd, _mm256_setzero_si256());
        u32sqdacc = _mm256_add_epi32(u32sqdacc, u32sqd_hi);
    };

    // 将`u32sqdacc`转移到`sqdacc`
    auto dump_u32sqdacc = [&]() mutable {
        __m256i u64sqdacc_p0 = _mm256_cvtepu32_epi64(_mm256_extractf128_si256(u32sqdacc, 0));
        __m256i u64sqdacc_p1 = _mm256_cvtepu32_epi64(_mm256_extractf128_si256(u32sqdacc, 1));
        __m256i u64sqdacc = _mm256_add_epi64(u64sqdacc_p0, u64sqdacc_p1);
        auto* tmp = (uint64_t*)&u64sqdacc;
        sqdacc += (tmp[0] + tmp[1] + tmp[2] + tmp[3]);
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
            dump_u32sqdacc();
            u32sqdacc = _mm256_setzero_si256();
            count = 0;
        }
    }

    dump_u32sqdacc();

    return sqdacc;
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
