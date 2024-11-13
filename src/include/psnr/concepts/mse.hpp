#pragma once

#include <concepts>

namespace psnr::concepts {

template <typename Self>
concept CMseOp =
    std::is_unsigned_v<typename Self::Tv> && requires(const Self::Tv* lhs, const Self::Tv* rhs, const size_t len) {
        { Self::mse(lhs, rhs, len) } -> std::floating_point;
    };

} // namespace psnr::concepts
