#pragma once

#include <cmath>
#include <concepts>

#include "psnr/concepts.hpp"
#include "psnr/helper/constexpr/math.hpp"
#include "psnr/impl/mse/v1.hpp"

namespace psnr::_psnr {

template <concepts::CMseOp TOp, typename Tv, size_t depth = sizeof(Tv) * 8>
    requires std::is_same_v<typename TOp::Tv, Tv>
[[nodiscard]] static inline double PsnrOp(const Tv* lhs, const Tv* rhs, const size_t len)
{
    const double mse = TOp()(lhs, rhs, len);
    constexpr uint64_t maxval = 1 << depth;
    const double psnr = 10 * std::log10((double)_hp::pow2(maxval) / mse);
    return psnr;
}

template double PsnrOp<psnr::mse::v1::MseOpu8>(const uint8_t* lhs, const uint8_t* rhs, const size_t len);
template double PsnrOp<psnr::mse::v2::MseOpu8>(const uint8_t* lhs, const uint8_t* rhs, const size_t len);

} // namespace psnr::_psnr

namespace psnr::psnr {

namespace _ = _psnr;

using _::PsnrOp;

} // namespace psnr::psnr
