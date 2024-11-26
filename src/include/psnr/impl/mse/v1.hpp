#pragma once

#include <concepts>
#include <cstdint>

#include "psnr/helper/constexpr/math.hpp"

namespace psnr::_mse::v1 {

template <std::unsigned_integral Tv_>
class MseOp_
{
public:
    using Tv = Tv_;

    [[nodiscard]] static inline double mse(const Tv* lhs, const Tv* rhs, const size_t len) noexcept
    {
        uint64_t sqrdiff_acc = sqrdiff(lhs, rhs, len);
        const double mse = (double)sqrdiff_acc / (double)len;
        return mse;
    }

    [[nodiscard]] static inline uint64_t sqrdiff(const Tv* lhs, const Tv* rhs, size_t len) noexcept;
};

template <std::unsigned_integral Tv>
uint64_t MseOp_<Tv>::sqrdiff(const Tv* lhs, const Tv* rhs, size_t len) noexcept
{
    uint32_t acc = 0;
    for (size_t i = 0; i < len; i++) {
        const uint32_t diff = lhs[i] - rhs[i];
        acc += diff * diff;
    }
    return acc;
}

using MseOpu8 = MseOp_<uint8_t>;

} // namespace psnr::_mse::v1

namespace psnr::mse::v1 {

namespace _ = _mse::v1;

using _::MseOp_;
using _::MseOpu8;

} // namespace psnr::mse::v1
