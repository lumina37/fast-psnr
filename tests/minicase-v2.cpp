#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "psnr/impl/mse/v2.hpp"

int main()
{
    constexpr auto width = 1920;
    constexpr auto height = 1080;
    constexpr size_t ysize = width * height;
    const uint8_t* lhs = (uint8_t*)std::malloc(ysize);
    const uint8_t* rhs = (uint8_t*)std::malloc(ysize);

    uint64_t sqrdiff = psnr::mse::v2::MseOpu8::sqrdiff(lhs, rhs, ysize);

    std::cout << sqrdiff << std::endl;
}
