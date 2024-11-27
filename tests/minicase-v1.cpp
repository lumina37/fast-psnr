#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "psnr/impl/mse/v1.hpp"

int main()
{
    constexpr size_t size = 2ull * 1024 * 1024 * 1024;
    auto* lhs = (uint8_t*)std::malloc(size);
    std::memset(lhs, 0, size);
    auto* rhs = (uint8_t*)std::malloc(size);
    std::memset(lhs, 20, size);

    uint64_t sqrdiff = psnr::mse::v1::MseOpu8::sqrdiff(lhs, rhs, size);

    std::cout << sqrdiff << std::endl;
}
