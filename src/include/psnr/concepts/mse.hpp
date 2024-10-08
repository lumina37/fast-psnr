#pragma once

#include <concepts>

namespace psnr::concepts {

template <typename TMseOp>
concept CMseOp = std::is_unsigned_v<typename TMseOp::Tv> &&
                 requires(const TMseOp::Tv* plhs, const TMseOp::Tv* prhs, const size_t len) {
                     { std::declval<TMseOp>()(plhs, prhs, len) } -> std::floating_point;
                 };

} // namespace psnr::concepts
