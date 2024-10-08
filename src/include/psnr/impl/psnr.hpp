#pragma once

#include <concepts>

#include "psnr/concepts.hpp"
#include "psnr/helper/constexpr/math.hpp"
#include "psnr/impl/mse/v1.hpp"

namespace psnr::_psnr {

template <typename TOp, typename Tv, size_t depth = sizeof(Tv) * 8>
    requires concepts::CMseOp<TOp> && std::is_same_v<typename TOp::Tv, Tv>
[[nodiscard]] static inline double PsnrOp(const Tv* plhs, const Tv* prhs, const size_t len)
{
    const double mse = TOp()(plhs, prhs, len);
    constexpr uint64_t maxval = 1 << depth;
    const double psnr = 10 * std::log10((double)_hp::pow2(maxval) / mse);
    return psnr;
}

} // namespace psnr::_psnr

namespace psnr::psnr {

namespace _ = _psnr;

using _::PsnrOp;

} // namespace psnr::psnr