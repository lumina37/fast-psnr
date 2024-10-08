#pragma once

#include <concepts>

namespace psnr::concepts {

template <typename TMseOp>
concept CMseOp = std::is_unsigned_v<typename TMseOp::Tv> &&
                 requires(const TMseOp::Tv* lhs, const TMseOp::Tv* rhs, const size_t len) {
                     { std::declval<TMseOp>()(lhs, rhs, len) } -> std::floating_point;
                 };

} // namespace psnr::concepts
