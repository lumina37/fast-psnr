#pragma once

#include <concepts>

#include "psnr/common/defines.h"
#include "psnr/helper/constexpr/math.hpp"

namespace psnr::_mse::v1 {

template <typename Tv_>
    requires std::is_unsigned_v<Tv_>
class MseOp
{
public:
    using Tv = Tv_;

    [[nodiscard]] inline double operator()(const Tv* lhs, const Tv* rhs, const size_t len) const noexcept
    {
        uint64_t acc = 0;

        const Tv* lhs_cursor = lhs;
        const Tv* rhs_cursor = rhs;
        for (size_t i = 0; i < len; i++) {
            int64_t l = *lhs_cursor;
            int64_t r = *rhs_cursor;
            int64_t diff = l - r;
            acc += _hp::pow2(diff);
            lhs_cursor++;
            rhs_cursor++;
        }

        const double mse = (double)acc / (double)len;
        return mse;
    }

    [[nodiscard]] static inline uint64_t sqrdiff(const Tv* lhs, const Tv* rhs, const size_t len) noexcept
    {
        const Tv* lhs_cursor = lhs;
        const Tv* rhs_cursor = rhs;
        uint64_t acc = 0;
        for (size_t i = 0; i < len; i++) {
            int64_t l = *lhs_cursor;
            int64_t r = *rhs_cursor;
            int64_t diff = l - r;
            acc += _hp::pow2(diff);
            lhs_cursor++;
            rhs_cursor++;
        }
        return acc;
    }
};

} // namespace psnr::_mse::v1

namespace psnr::mse::v1 {

namespace _ = _mse::v1;

using _::MseOp;

} // namespace psnr::mse::v1
